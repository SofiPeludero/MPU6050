#include <Wire.h>

const int MPU6050_ADDR = 0x68;    // Dirección I2C del MPU6050
const int ACCEL_XOUT_H = 0x3B;    // Registro de datos de aceleración en X (byte alto)
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
  float gX = accelX / 16384.0; // 16384 es la relación para ±2g
  float gY = accelY / 16384.0;
  float gZ = accelZ / 16384.0;

  // Mostrar los resultados
  Serial.print("Aceleración en X: ");
  Serial.print(gX);
  Serial.print(" g, Y: ");
  Serial.print(gY);
  Serial.print(" g, Z: ");
  Serial.print(gZ);
  Serial.println(" g");

  delay(500);  // Pequeña pausa entre lecturas
}
