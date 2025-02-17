import sys
import struct
import serial
import serial.tools.list_ports
from PyQt6.QtWidgets import QApplication, QWidget, QVBoxLayout, QLabel, QPushButton, QGroupBox, QComboBox, QHBoxLayout, QMessageBox
from PyQt6.QtCore import QThread, pyqtSignal, QTimer

class PacketParserThread(QThread):
    parsed_data = pyqtSignal(dict)

    def __init__(self):
        super().__init__()
        self.running = True
        self.packet_queue = []

    def run(self):
        while self.running:
            if self.packet_queue:
                packet = self.packet_queue.pop(0)
                self.parse_packet(packet)
            self.msleep(1)  # Nghỉ ngắn để tránh chiếm CPU quá mức

    def parse_packet(self, packet):
        print("Parser: Received packet:", ' '.join(f'{b:02X}' for b in packet))

        command_length = packet[1]
        command = (command_length >> 5) & 0x07  # 3-bit command
        length = (command_length & 0x1F) + 1    # 5-bit length (minimum length of the valid packet is 1 byte data)

        data = packet[2:2+length]
        crc = struct.unpack("<H", packet[2+length:4+length])[0]

        parsed_output = {
            "command": command,
            "length": length,
            "data": data,
            "crc": crc
        }
        print("Parser: Successfully parsed packet:", parsed_output)
        self.parsed_data.emit(parsed_output)


    def add_packet(self, packet):
        self.packet_queue.append(packet)

    def stop(self):
        self.running = False
        self.quit()
        self.wait()


class SerialReaderThread(QThread):
    data_received = pyqtSignal(bytearray)

    def __init__(self, serial_port):
        super().__init__()
        self.serial_port = serial_port
        self.running = True
        self.state = "IDLE"
        self.packet = bytearray()
        self.expected_length = 0

    def run(self):
        while self.running:
            if self.serial_port.in_waiting:
                byte = self.serial_port.read(1)
                self.process_byte(byte)
            self.msleep(1)  # Nghỉ ngắn để giảm tải CPU

    def process_byte(self, byte):
        byte_val = byte[0]
        # Sử dụng state machine để đọc packet
        if self.state == "IDLE":
            if byte_val == 0xAA:
                self.packet = bytearray([byte_val])
                self.state = "WAIT_CMD_LEN"
        elif self.state == "WAIT_CMD_LEN":
            self.packet.append(byte_val)
            self.expected_length = byte_val & 0x1F
            self.state = "RECEIVE_PAYLOAD"
        elif self.state == "RECEIVE_PAYLOAD":
            self.packet.append(byte_val)
            # Chờ nhận đủ payload + 2 byte CRC: tổng số byte cần = 2 + expected_length + 2
            if len(self.packet) == 2 + self.expected_length + 2:
                self.state = "WAIT_EOP"
        elif self.state == "WAIT_EOP":
            self.packet.append(byte_val)
            if byte_val == 0x55:
                self.state = "VALID_PACKET"
                # Emit một bản copy của packet để đảm bảo dữ liệu không thay đổi sau này
                self.data_received.emit(self.packet.copy())
            else:
                self.state = "WAIT_EOP_AGAIN"
        elif self.state == "WAIT_EOP_AGAIN":
            self.packet.append(byte_val)
            if byte_val == 0x55:
                self.state = "VALID_PACKET"
                self.data_received.emit(self.packet.copy())
            else:
                self.state = "INVALID_PACKET"
                self.data_received.emit(self.packet.copy())

        # Sau khi nhận được packet hợp lệ hoặc không hợp lệ, reset lại state machine
        if self.state in ["VALID_PACKET", "INVALID_PACKET"]:
            self.state = "IDLE"
            self.packet = bytearray()

        print(f"Received packet: {' '.join(f'{b:02X}' for b in self.packet)}, State: {self.state}")

    def stop(self):
        self.running = False
        self.quit()
        self.wait()


class MyWindow(QWidget):
    def __init__(self):
        super().__init__()
        self.initUI()
        self.serial_port = None
        self.reader_thread = None
        self.parser_thread = PacketParserThread()
        self.parser_thread.parsed_data.connect(self.display_packet)
        self.parser_thread.start()
        self.com_refresh_timer = QTimer(self)
        self.com_refresh_timer.timeout.connect(self.refresh_com_ports)
        self.com_refresh_timer.start(5000)

    def initUI(self):
        self.setWindowTitle("USB Packet Receiver")
        self.setGeometry(100, 100, 500, 400)
        self.layout = QVBoxLayout()

        self.com_group = QGroupBox("Select COM Port")
        self.com_layout = QHBoxLayout()
        self.com_selector = QComboBox()
        self.refresh_com_ports()
        self.com_layout.addWidget(self.com_selector)

        self.connect_button = QPushButton("Connect")
        self.connect_button.clicked.connect(self.connect_serial)
        self.com_layout.addWidget(self.connect_button)

        self.disconnect_button = QPushButton("Disconnect")
        self.disconnect_button.clicked.connect(self.disconnect_serial)
        self.com_layout.addWidget(self.disconnect_button)

        self.com_group.setLayout(self.com_layout)
        self.layout.addWidget(self.com_group)

        self.label_command = QLabel("COMMAND: ")
        self.layout.addWidget(self.label_command)

        self.label_length = QLabel("LENGTH: ")
        self.layout.addWidget(self.label_length)

        self.label_data = QLabel("DATA: ")
        self.layout.addWidget(self.label_data)

        self.label_crc = QLabel("CRC: ")
        self.layout.addWidget(self.label_crc)

        self.status_label = QLabel("Status: Disconnected")
        self.layout.addWidget(self.status_label)

        self.setLayout(self.layout)

    def refresh_com_ports(self):
        current_port = self.com_selector.currentText()
        ports = [port.device for port in serial.tools.list_ports.comports()]
        self.com_selector.clear()
        self.com_selector.addItems(ports)
        if current_port in ports:
            self.com_selector.setCurrentText(current_port)

    def connect_serial(self):
        selected_port = self.com_selector.currentText()
        if not selected_port:
            QMessageBox.critical(self, "Error", "No COM port selected!")
            return
        try:
            self.serial_port = serial.Serial(selected_port, 115200, timeout=1)
            self.status_label.setText(f"Status: Connected to {selected_port}")
            self.reader_thread = SerialReaderThread(self.serial_port)
            # Khi kết nối signal, gửi bản copy của packet để đảm bảo dữ liệu không bị thay đổi
            self.reader_thread.data_received.connect(lambda packet: self.parser_thread.add_packet(packet.copy()))
            self.reader_thread.start()
        except serial.SerialException:
            QMessageBox.critical(self, "Error", "Failed to connect to the selected COM port.")

    def disconnect_serial(self):
        if self.reader_thread:
            self.reader_thread.stop()
            self.reader_thread = None
        if self.serial_port:
            self.serial_port.close()
            self.serial_port = None
        self.status_label.setText("Status: Disconnected")

    def display_packet(self, packet):
        self.label_command.setText(f"COMMAND: {packet['command']:02X}")
        self.label_length.setText(f"LENGTH: {packet['length']}")
        self.label_data.setText(f"DATA: {' '.join(f'{b:02X}' for b in packet['data'])}")
        self.label_crc.setText(f"CRC: {packet['crc']:04X}")

    def closeEvent(self, event):
        self.disconnect_serial()
        self.parser_thread.stop()
        event.accept()


if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = MyWindow()
    window.show()
    sys.exit(app.exec())
