import serial
import csv

# Arduino 포트 및 속도 설정
port = "/dev/cu.usbmodem21401"  # Arduino 연결된 포트로 수정
# /dev/cu.usbmodem21401
baudrate = 9600
arduino = serial.Serial(port, baudrate, timeout=1)

# 데이터 저장용 리스트
sensor_data = []

print("Connected to:", arduino.name)  # 연결된 포트 출력
print("Data collection started. Press Ctrl+C to stop.")

try:
    while True:
        if arduino.in_waiting > 0:
            data = arduino.readline().decode('utf-8').strip()
            print(f"Received: {data}")  # 실시간 출력
            sensor_data.append(data)  # 데이터를 리스트에 저장

except KeyboardInterrupt:
    print("\nData collection stopped by Ctrl+C.")
    
    # CSV 파일에 데이터 저장
    output_file = "sensor_values_test.csv"
    with open(output_file, mode='w', newline='') as file:
        writer = csv.writer(file)
        writer.writerow(["SensorValue"])  # 헤더 작성
        for value in sensor_data:
            writer.writerow([value])  # 수집된 값 저장
    
    print(f"Data saved to {output_file}.")

finally:
    arduino.close()
    print("Arduino connection closed.")
