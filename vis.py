import pandas as pd
import matplotlib.pyplot as plt

# CSV 파일 읽기
file_path = "sensor_values_test.csv"  # 업로드된 파일 경로로 수정
sensor_data = pd.read_csv(file_path)

# 'Sensor Value'와 'Percentage' 값 추출
sensor_data[['Sensor Value', 'Percentage']] = sensor_data['SensorValue'].str.extract(
    r'Sensor Value: (\d+), Percentage: (\d+)'  # 정규 표현식을 사용해 값 추출
)

# 'Percentage'를 정수형으로 변환
sensor_data['Percentage'] = pd.to_numeric(sensor_data['Percentage'], errors='coerce')

# 데이터 분리: 색상에 따라 선분을 나눔
x = range(len(sensor_data['Percentage']))
y = sensor_data['Percentage']

# 시각화
plt.figure(figsize=(10, 6))

# 선 색상 조건별로 나누기
for i in range(1, len(y)):
    if y[i] >= 90:
        plt.plot(x[i-1:i+1], y[i-1:i+1], color='red', linewidth=2)
    elif y[i] >= 80:
        plt.plot(x[i-1:i+1], y[i-1:i+1], color='orange', linewidth=2)
    else:
        plt.plot(x[i-1:i+1], y[i-1:i+1], color='blue', linewidth=2)

# 그래프 꾸미기
plt.axhline(80, color='orange', linestyle='--', label='Threshold: 80%')
plt.axhline(90, color='red', linestyle='--', label='Threshold: 90%')
plt.xlabel('Time Step')
plt.ylabel('Percentage')
plt.title('Sensor Percentage Over Time')
plt.legend()
plt.grid()
plt.show()
