#include <Wire.h>

const int MPU6050_ADDR = 0x68;    // Dirección I2C del MPU6050
const int ACCEL_XOUT_H = 0x3B;    // Registro de datos de aceleración en X (byte alto)
const int GYRO_XOUT_H = 0x43;     // Registro de datos del giroscopio en X (byte alto)
const int PWR_MGMT_1 = 0x6B;      // Registro de gestión de energía

void setup() {
  Wire.begin(); // Inicializar I2C
  Serial.begin(115200); // Inicializar comunicación serial

  // Despertar el MPU6050 (por defecto está en modo sleep)
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(PWR_MGMT_1);  // Accede al registro de gestión de energía
  Wire.write(0);           // Establece el valor para despertar el sensor
  Wire.endTransmission(true);
}

void loop() {
  int16_t accelX, accelY, accelZ;
  int16_t gyroX, gyroY, gyroZ;
  
  // Leer los valores de aceleración
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(ACCEL_XOUT_H); // Accede al primer registro de aceleración
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050_ADDR, 6, true); // Lee 6 bytes (2 por cada eje)

   // Combinar los bytes alto y bajo para cada eje
  accelX = (Wire.read() << 8 | Wire.read()); // X
  accelY = (Wire.read() << 8 | Wire.read()); // Y
  accelZ = (Wire.read() << 8 | Wire.read()); // Z

   // Convertir a valores en g's (gravedad terrestre) si es necesario
  // Los valores del MPU6050 son en enteros de 16 bits (±2g en este caso)
  float aX = (accelX)/ 16384.0; // 16384 es la relación para ±2g
  float aY = (accelY)/ 16384.0;
  float aZ = (accelZ-2048) / 16384.0;

  // Leer los valores de giroscopio
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(GYRO_XOUT_H); // Accede al primer registro de giroscopio
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050_ADDR, 6, true); // Lee 6 bytes (2 por cada eje)

  // Combinar los bytes alto y bajo para cada eje
  gyroX = (Wire.read() << 8 | Wire.read()); // X
  gyroY = (Wire.read() << 8 | Wire.read()); // Y
  gyroZ = (Wire.read() << 8 | Wire.read()); // Z

  // Convertir los valores de giroscopio a grados/segundo
  // La sensibilidad es de 131 LSB/(°/s) para un rango de ±250°/s
  float gX = gyroX / 131.0;
  float gY = gyroY / 131.0;
  float gZ = gyroZ / 131.0;
 

  // Mostrar los resultados
  Serial.print("\nAceleración en X SIN PROCESAR : \n X");
  Serial.print(accelX);
  Serial.print("\t Y: ");
  Serial.print(accelY);
  Serial.print("\t Z: ");
  Serial.print(accelZ);
  Serial.println("\n");
  
  Serial.print("Aceleración real - deseada = offsets \n X:");
  Serial.print(0 - accelX);
  Serial.print("\t Y: ");
  Serial.print(0 - accelY);
  Serial.print("\t Z: ");
  Serial.print(16384 - accelZ);
  Serial.println("\n");
  
  
  Serial.print("Aceleración COOREGIDA \n X:");
  Serial.print(aX*9.8);
  Serial.print(" m/s2, Y: ");
  Serial.print(aY*9.8);
  Serial.print(" m/s2, Z: ");
  Serial.print(aZ*9.8);
  Serial.println(" m/s2 ");

  Serial.print("\n Angulo en X: \n");
  Serial.print(aX*9.8);
  Serial.print("°, Y: ");
  Serial.print(aY*9.8);
  Serial.print("°, Z: ");
  Serial.print(aZ*9.8);
  Serial.println("°");

  delay(1000);  // Pequeña pausa entre lecturas
}
