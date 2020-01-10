/*
 * Date created   - 12/11/2019
 * Author 		  - Ajay
 *

 * MPU9250.c
 *
 */
#include "MPU9250.h"


//1- I2C Handle
static I2C_HandleTypeDef i2cHandler;


//2- Accel & Gyro Scaling Factor
static float accelScalingFactor, gyroScalingFactor;


//3- Bias varaibles
static float A_X_Bias = 0.0f;
static float A_Y_Bias = 0.0f;
static float A_Z_Bias = 0.0f;

static int16_t GyroRW[3];

//Function Definitions


//1- i2c Handler
void MPU9250_Init(I2C_HandleTypeDef *I2Chnd)
{
	//Copy I2C CubeMX handle to local library
	memcpy(&i2cHandler, I2Chnd, sizeof(*I2Chnd));
}

//2- i2c Read
void I2C_Read(uint8_t ADDR, uint8_t *i2cBif, uint8_t NofData)
{
	uint8_t i2cBuf[2];
	uint8_t MPUADDR;
	//Need to Shift address to make it proper to i2c operation
	MPUADDR = (MPU_ADDR<<1);
	i2cBuf[0] = ADDR;
	HAL_I2C_Master_Transmit(&i2cHandler, MPUADDR, i2cBuf, 1, 10);
	HAL_I2C_Master_Receive(&i2cHandler, MPUADDR, i2cBif, NofData, 100);
}

/*
 *  API 			- I2C write API
 *  param1			- Address of the slave device
 *  param2			- Data byte
 *
 *  ret val			- NONE
 *
 *  NOTE			- None
 */
void I2C_Write8(uint8_t ADDR, uint8_t data)
{
	uint8_t i2cData[2];
	i2cData[0] = ADDR;
	i2cData[1] = data;
	uint8_t MPUADDR = (MPU_ADDR<<1);
	HAL_I2C_Master_Transmit(&i2cHandler, MPUADDR, i2cData, 2,100);
}




/*
 *  API 			- MPU9250 Initialization configuration API
 *  param1			- Address of the structure variable to the MPU config typedef struct
 *  param2			- NULL
 *
 *  ret val			- NONE
 *
 *  NOTE			- None
 */
void MPU9250_Config(MPU_ConfigTypeDef *config)
{
	uint8_t Buffer = 0;
	//Clock Source
	//Reset Device
	I2C_Write8(PWR_MAGT_1_REG, 0x80);
	HAL_Delay(100);
	Buffer = config ->ClockSource & 0x07; //change the 7th bits of register
	Buffer |= (config ->Sleep_Mode_Bit << 6) &0x40; // change only the 7th bit in the register
	I2C_Write8(PWR_MAGT_1_REG, Buffer);
	HAL_Delay(100); // should wait 10ms after changing the clock setting.

	//Set the Digital Low Pass Filter
	Buffer = 0;
	Buffer = config->CONFIG_DLPF & 0x07;
	I2C_Write8(CONFIG_REG, Buffer);

	//Select the Gyroscope Full Scale Range
	Buffer = 0;
	Buffer = (config->Gyro_Full_Scale << 3) & 0x18;
	I2C_Write8(GYRO_CONFIG_REG, Buffer);

	//Select the Accelerometer Full Scale Range
	Buffer = 0;
	Buffer = (config->Accel_Full_Scale << 3) & 0x18;
	I2C_Write8(ACCEL_CONFIG_REG, Buffer);



	//Set SRD To Default
	MPU9250_Set_SMPRT_DIV(0x04);


	//Accelerometer Scaling Factor, Set the Accelerometer and Gyroscope Scaling Factor
	switch (config->Accel_Full_Scale)
	{
		case AFS_SEL_2g:
			accelScalingFactor = (2000.0f/32768.0f);
			break;

		case AFS_SEL_4g:
			accelScalingFactor = (4000.0f/32768.0f);
				break;

		case AFS_SEL_8g:
			accelScalingFactor = (8000.0f/32768.0f);
			break;

		case AFS_SEL_16g:
			accelScalingFactor = (16000.0f/32768.0f);
			break;

		default:
			break;
	}
	//Gyroscope Scaling Factor
	switch (config->Gyro_Full_Scale)
	{
		case FS_SEL_250:
			gyroScalingFactor = 250.0f/32768.0f;
			break;

		case FS_SEL_500:
				gyroScalingFactor = 500.0f/32768.0f;
				break;

		case FS_SEL_1000:
			gyroScalingFactor = 1000.0f/32768.0f;
			break;

		case FS_SEL_2000:
			gyroScalingFactor = 2000.0f/32768.0f;
			break;

		default:
			break;
	}

}


/*
 *  API 			- Get sample rate divider
 *  param1			- NULL
 *  param2			- NULL
 *
 *  ret val			- buffer value
 *
 *  NOTE			- None
 */
uint8_t MPU9250_Get_SMPRT_DIV(void)
{
	uint8_t Buffer = 0;

	I2C_Read(SMPLRT_DIV_REG, &Buffer, 1);
	return Buffer;
}






/*
 *  API 			- Set sample rate divider
 *  param1			- sample rate value
 *  param2			- NULL
 *
 *  ret val			- NULL
 *
 *  NOTE			- None
 */
void MPU9250_Set_SMPRT_DIV(uint8_t SMPRTvalue)
{
	I2C_Write8(SMPLRT_DIV_REG, SMPRTvalue);
}







/*
 *  API 			- Get external frame sync
 *  param1			- NULL
 *  param2			- NULL
 *
 *  ret val			- buffer value >> 3
 *
 *  NOTE			- NULL
 */
uint8_t MPU9250_Get_FSYNC(void)
{
	uint8_t Buffer = 0;

	I2C_Read(CONFIG_REG, &Buffer, 1);
	Buffer &= 0x38;
	return (Buffer>>3);
}






/*
 *  API 			- Set external frame sync
 *  param1			- EXT_SYNC_SET_ENUM type variable
 *  param2			- NULL
 *
 *  ret val			- None
 *
 *  NOTE			- NULL
 */
void MPU9250_Set_FSYNC(enum EXT_SYNC_SET_ENUM ext_Sync)
{
	uint8_t Buffer = 0;
	I2C_Read(CONFIG_REG, &Buffer,1);
	Buffer &= ~0x38;

	Buffer |= (ext_Sync <<3);
	I2C_Write8(CONFIG_REG, Buffer);

}

/*
 *  API 			- get raw acceleration data
 *  param1			- handle to struct RawData_Def
 *  param2			- NULL
 *
 *  ret val			- None
 *
 *  NOTE			- NULL
 */
void MPU9250_Get_Accel_RawData(RawData_Def *rawDef)
{
	uint8_t i2cBuf[2];
	uint8_t AcceArr[6], GyroArr[6];

	I2C_Read(INT_STATUS_REG, &i2cBuf[1],1);
	if((i2cBuf[1]&&0x01))
	{
		I2C_Read(ACCEL_XOUT_H_REG, AcceArr,6);

		//Accel Raw Data
		rawDef->x = ((AcceArr[0]<<8) + AcceArr[1]); // x-Axis
		rawDef->y = ((AcceArr[2]<<8) + AcceArr[3]); // y-Axis
		rawDef->z = ((AcceArr[4]<<8) + AcceArr[5]); // z-Axis


		//Gyro Raw Data
		I2C_Read(GYRO_XOUT_H_REG, GyroArr,6);
		GyroRW[0] = ((GyroArr[0]<<8) + GyroArr[1]);
		GyroRW[1] = (GyroArr[2]<<8) + GyroArr[3];
		GyroRW[2] = ((GyroArr[4]<<8) + GyroArr[5]);
	}
}

//10- Get Accel scaled data (g unit of gravity, 1g = 9.81m/s2)
void MPU9250_Get_Accel_Scale(ScaledData_Def *scaledDef)
{

	RawData_Def AccelRData;
	MPU9250_Get_Accel_RawData(&AccelRData);

	//Accel Scale data
	scaledDef->x = ((AccelRData.x+0.0f)*accelScalingFactor);
	scaledDef->y = ((AccelRData.y+0.0f)*accelScalingFactor);
	scaledDef->z = ((AccelRData.z+0.0f)*accelScalingFactor);
}

//11- Get Accel calibrated data
void MPU9250_Get_Accel_Cali(ScaledData_Def *CaliDef)
{
	ScaledData_Def AccelScaled;
	MPU9250_Get_Accel_Scale(&AccelScaled);

	//Accel Scale data
	CaliDef->x = (AccelScaled.x) - A_X_Bias; // x-Axis
	CaliDef->y = (AccelScaled.y) - A_Y_Bias;// y-Axis
	CaliDef->z = (AccelScaled.z) - A_Z_Bias;// z-Axis
}
//12- Get Gyro Raw Data
void MPU9250_Get_Gyro_RawData(RawData_Def *rawDef)
{

	//Accel Raw Data
	rawDef->x = GyroRW[0];
	rawDef->y = GyroRW[1];
	rawDef->z = GyroRW[2];

}

//13- Get Gyro scaled data
void MPU9250_Get_Gyro_Scale(ScaledData_Def *scaledDef)
{
	RawData_Def myGyroRaw;
	MPU9250_Get_Gyro_RawData(&myGyroRaw);

	//Gyro Scale data
	scaledDef->x = (myGyroRaw.x)*gyroScalingFactor; // x-Axis
	scaledDef->y = (myGyroRaw.y)*gyroScalingFactor; // y-Axis
	scaledDef->z = (myGyroRaw.z)*gyroScalingFactor; // z-Axis
}

//14- Accel Calibration
void _Accel_Cali(float x_min, float x_max, float y_min, float y_max, float z_min, float z_max)
{
	//1* X-Axis calibrate
	A_X_Bias		= (x_max + x_min)/2.0f;

	//2* Y-Axis calibrate
	A_Y_Bias		= (y_max + y_min)/2.0f;

	//3* Z-Axis calibrate
	A_Z_Bias		= (z_max + z_min)/2.0f;
}
