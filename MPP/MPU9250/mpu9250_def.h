#ifndef MPU9250_DEF_HEADER
#define MPU9250_DEF_HEADER


#define MPU9250_DEGTORAD								57.2957795131


#define MPU9250_AK8963_I2C_ADDR             			0x0C
#define MPU9250_AK8963_DEVID            				0x48
#define MPU9250_AK8963_READ             				0x80

// Read-only Reg
#define MPU9250_AK8963_WIA                				0x00
#define MPU9250_AK8963_INFO         			        0x01
#define MPU9250_AK8963_ST1 								0x02
#define MPU9250_AK8963_HXL 								0x03
#define MPU9250_AK8963_HXH        						0x04
#define MPU9250_AK8963_HYL    							0x05
#define MPU9250_AK8963_HYH         						0x06
#define MPU9250_AK8963_HZL          			        0x07
#define MPU9250_AK8963_HZH          			        0x08
#define MPU9250_AK8963_ST2                  			0x09

// Write/Read Reg
#define MPU9250_AK8963_CNTL1                			0x0A
#define MPU9250_AK8963_CNTL2                			0x0B
#define MPU9250_AK8963_ASTC                 			0x0C
#define MPU9250_AK8963_TS1                  			0x0D
#define MPU9250_AK8963_TS2                  			0x0E
#define MPU9250_AK8963_I2CDIS               			0x0F

// Read-only Reg ( ROM )
#define MPU9250_AK8963_ASAX                 			0x10
#define MPU9250_AK8963_ASAY                 			0x11
#define MPU9250_AK8963_ASAZ                 			0x12



// Register 25 � Sample Rate Divider
#define MPU9250_REGISTER_SMPLRT_DIV						25

// Register 26 � Configuration
#define MPU9250_REGISTER_CONFIG							26
#define MPU9250_REGISTER_CONFIG_FIFO_MODE				0b01000000	/* When set to �1�, when the fifo is full, additional writes will not be written to fifo. When set to �0�, when the fifo is full, additional writes will be written to the fifo, replacing the oldest data. */
#define MPU9250_REGISTER_CONFIG_EXT_SYNC_SET_MASK		0b11000111  /* Enables the FSYNC pin data to be sampled */
#define MPU9250_REGISTER_CONFIG_EXT_SYNC_SET_OFF		0b00000000
#define MPU9250_REGISTER_CONFIG_DLPF_CFG_MASK			0b11111000	/* For the DLPF to be used, fchoice[1:0] must be set to 2�b11, fchoice_b[1:0] is 2�b00. */

// Register 27 � Gyroscope Configuration
#define MPU9250_REGISTER_GYRO_CONFIG					27
#define MPU9250_REGISTER_GYRO_CONFIG_XGYRO_CTEN			0b10000000	/* X Gyro self-test */
#define MPU9250_REGISTER_GYRO_CONFIG_YGYRO_CTEN			0b01000000	/* Y Gyro self-test */
#define MPU9250_REGISTER_GYRO_CONFIG_ZGYRO_CTEN			0b00100000	/* Z Gyro self-test */
#define MPU9250_REGISTER_GYRO_CONFIG_GYRO_FS_SEL_MASK	0b11100111	/* Gyro Full Scale Select */
#define MPU9250_REGISTER_GYRO_CONFIG_GYRO_FCOISE_B_MASK	0b11111100	/* Used to bypass DLPF as shown in table 1 above. NOTE: Register is Fchoice_b (inverted version of Fchoice), table 1 uses Fchoice (which is the inverted version of this register). */

// Register 28 � Accelerometer Configuration
#define MPU9250_REGISTER_ACCEL_CONFIG					28
#define MPU9250_REGISTER_ACCEL_CONFIG_AX_ST_EN			0b10000000	/* X Accel self-test */
#define MPU9250_REGISTER_ACCEL_CONFIG_AY_ST_EN			0b01000000	/* Y Accel self-test */
#define MPU9250_REGISTER_ACCEL_CONFIG_AZ_ST_EN			0b00100000	/* Z Accel self-test */
#define MPU9250_REGISTER_ACCEL_CONFIG_ACCEL_FS_SEL_MASK	0b11100111	/* Accel Full Scale Select */
#define MPU9250_REGISTER_ACCEL_CONFIG_ACCEL_FS_SEL_2G	0b00000000
#define MPU9250_REGISTER_ACCEL_CONFIG_ACCEL_FS_SEL_4G	0b00001000
#define MPU9250_REGISTER_ACCEL_CONFIG_ACCEL_FS_SEL_8G	0b00010000
#define MPU9250_REGISTER_ACCEL_CONFIG_ACCEL_FS_SEL_16G	0b00011000

// Register 29 � Accelerometer Configuration 2
#define MPU9250_REGISTER_ACCEL_CONFIG2					29
#define MPU9250_REGISTER_ACCEL_CONFIG2_FCHOISE_B		0b00001000
#define MPU9250_REGISTER_ACCEL_CONFIG2_ADLPFCFG_MASK	0b11111000




#define MPU9250_REGISTER_ZRMOT_THR						33

#define MPU9250_REGISTER_ZRMOT_DUR						34

// Register 35 � FIFO Enable
#define MPU9250_REGISTER_FIFO_EN						35
#define MPU9250_REGISTER_FIFO_EN_TEMP					0b10000000
#define MPU9250_REGISTER_FIFO_EN_GYRO					0b01110000
#define MPU9250_REGISTER_FIFO_EN_ACCEL					0b00001000
#define MPU9250_REGISTER_FIFO_EN_SLV					0b00000111

#define MPU9250_REGISTER_I2C_MST_CTRL					36
#define MPU9250_REGISTER_I2C_MST_CTRL_CLK_MASK			0b00001111
#define MPU9250_REGISTER_I2C_MST_CTRL_MULT_MST_EN		0b10000000
#define MPU9250_REGISTER_I2C_MST_CTRL_I2C_MST_P_NSR		0b00001000


#define MPU9250_REGISTER_I2C_SLV0_ADDR					37
#define MPU9250_REGISTER_I2C_SLV0_REG					38
#define MPU9250_REGISTER_I2C_SLV0_CTRL					39

#define MPU9250_REGISTER_I2C_SLV4_ADDR					49
#define MPU9250_REGISTER_I2C_SLV4_REG					50
#define MPU9250_REGISTER_I2C_SLV4_DO					51
#define MPU9250_REGISTER_I2C_SLV4_CTRL					52
#define MPU9250_REGISTER_I2C_SLV4_DI					53

// Register 55 � INT Pin / Bypass Enable Configuration
#define MPU9250_REGISTER_INT_PIN_CFG					55
#define MPU9250_REGISTER_INT_PIN_CFG_ACTL				0b10000000
#define MPU9250_REGISTER_INT_PIN_CFG_OPEN				0b01000000
#define MPU9250_REGISTER_INT_PIN_CFG_LATCH_INT_EN		0b00100000
#define MPU9250_REGISTER_INT_PIN_CFGINT_ANYRD_2CLEAR	0b00010000
#define MPU9250_REGISTER_INT_PIN_ACTL_FSYNC				0b00001000
#define MPU9250_REGISTER_INT_PIN_FSYNC_INT_MODE_EN		0b00000100
#define MPU9250_REGISTER_INT_PIN_BYPASS_EN				0b00000010

// Register 56 � Interrupt Enable
#define MPU9250_REGISTER_INT_ENABLE						56
#define MPU9250_REGISTER_INT_ENABLE_DMP_INT_EN			0b00000010
#define MPU9250_REGISTER_INT_ENABLE_DATA_RDY_EN			0b00000001

#define MPU9250_REGISTER_INT_STATUS						58
#define MPU9250_REGISTER_INT_STATUS_DATA_RDY_EN			0b00000001

#define MPU9250_REGISTER_ACCEL_XOUT_H 					59
#define MPU9250_REGISTER_ACCEL_XOUT_L 					60
#define MPU9250_REGISTER_ACCEL_YOUT_H 					61
#define MPU9250_REGISTER_ACCEL_YOUT_L 					62
#define MPU9250_REGISTER_ACCEL_ZOUT_H 					63
#define MPU9250_REGISTER_ACCEL_ZOUT_L 					64

#define MPU9250_REGISTER_GYRO_XOUT_H 					67
#define MPU9250_REGISTER_GYRO_XOUT_L 					68
#define MPU9250_REGISTER_GYRO_YOUT_H 					69
#define MPU9250_REGISTER_GYRO_YOUT_L 					70
#define MPU9250_REGISTER_GYRO_ZOUT_H 					71
#define MPU9250_REGISTER_GYRO_ZOUT_L 					72

#define MPU9250_REGISTER_EXT_SENS_DATA_00				73
#define MPU9250_REGISTER_EXT_SENS_DATA_01				74
#define MPU9250_REGISTER_EXT_SENS_DATA_02				75
#define MPU9250_REGISTER_EXT_SENS_DATA_03				76
#define MPU9250_REGISTER_EXT_SENS_DATA_04				77
#define MPU9250_REGISTER_EXT_SENS_DATA_05				78
#define MPU9250_REGISTER_EXT_SENS_DATA_06				79
#define MPU9250_REGISTER_EXT_SENS_DATA_07				80
#define MPU9250_REGISTER_EXT_SENS_DATA_08				81
#define MPU9250_REGISTER_EXT_SENS_DATA_09				82
#define MPU9250_REGISTER_EXT_SENS_DATA_10				83
#define MPU9250_REGISTER_EXT_SENS_DATA_11				84
#define MPU9250_REGISTER_EXT_SENS_DATA_12				85
#define MPU9250_REGISTER_EXT_SENS_DATA_13				86
#define MPU9250_REGISTER_EXT_SENS_DATA_14				87
#define MPU9250_REGISTER_EXT_SENS_DATA_15				88
#define MPU9250_REGISTER_EXT_SENS_DATA_16				89
#define MPU9250_REGISTER_EXT_SENS_DATA_17				90
#define MPU9250_REGISTER_EXT_SENS_DATA_18				91
#define MPU9250_REGISTER_EXT_SENS_DATA_19				92
#define MPU9250_REGISTER_EXT_SENS_DATA_20				93
#define MPU9250_REGISTER_EXT_SENS_DATA_21				94
#define MPU9250_REGISTER_EXT_SENS_DATA_22				95
#define MPU9250_REGISTER_EXT_SENS_DATA_23				96

#define MPU9250_REGISTER_I2C_SLV0_DO					99
#define MPU9250_REGISTER_I2C_SLV1_DO					100


// Register 106 � User Control
#define MPU9250_REGISTER_USER_CTRL						106
#define MPU9250_REGISTER_USER_CTRL_FIFO_EN				0b01000000
#define MPU9250_REGISTER_USER_CTRL_I2C_MST_EN 			0b00100000
#define MPU9250_REGISTER_USER_CTRL_I2C_IF_DIS 			0b00010000
#define MPU9250_REGISTER_USER_CTRL_I2C_FIFO_RST 		0b00000100
#define MPU9250_REGISTER_USER_CTRL_I2C_MST_RST 			0b00000010
#define MPU9250_REGISTER_USER_CTRL_SIG_COND_RST			0b00000001

//  Register 107 � Power Management 1
#define MPU9250_REGISTER_PWR_MGMT_1 		 			107			/* This register allows the user to configure the power mode and clock source. It also provides a bit for resetting the entire device, and a bit for disabling the temperature sensor. */
#define MPU9250_REGISTER_PWR_MGMT_1_TEMP_DIS 			0b00001000  /* When set to 1, this bit disables the temperature sensor */
#define MPU9250_REGISTER_PWR_MGMT_1_CYCLE 				0b00100000  /* When this bit is set to 1 and SLEEP is disabled, the MPU-9150 will cycle between sleep mode and waking up to take a single sample of data from active sensors at a rate determined by LP_WAKE_CTRL (register 108). */
#define MPU9250_REGISTER_PWR_MGMT_1_SLEEP 				0b01000000	/* When set to 1, this bit puts the MPU-9150 into sleep mode. */
#define MPU9250_REGISTER_PWR_MGMT_1_DEVICE_RESET 		0b10000000	/* When set to 1, this bit resets all internal registers to their default values. */

#define MPU9250_REGISTER_PWR_MGMT_2 		 			108

#define MPU9250_REGISTER_FIFO_COUNT_H					114
#define MPU9250_REGISTER_FIFO_COUNT_L					115
#define MPU9250_REGISTER_FIFO_R_W						116

#define MPU9250_REGISTER_WHO_AM_I						117

#define MPU9250_CLOCKSOURCE_INTERNAL_8MHZ				0
#define MPU9250_CLOCKSOURCE_PLL_X_AXIS_GYRO_REF			1
#define MPU9250_CLOCKSOURCE_PLL_Y_AXIS_GYRO_REF			2
#define MPU9250_CLOCKSOURCE_PLL_Z_AXIS_GYRO_REF			3
#define MPU9250_CLOCKSOURCE_PLL_EXTERNAL_32768			4
#define MPU9250_CLOCKSOURCE_PLL_EXTERNAL_19200000		5
#define MPU9250_CLOCKSOURCE_STOP_CLOCK					7

#define MPU9250_CLOCKSOURCE_RESET						0b11111000
#define MPU9250_CLOCKSOURCE_MASK						0b00000111
#define MPU9250_GYRORANGE_RESET							0b11100111
#define MPU9250_GYRORANGE_MASK							0b00011000
#define MPU9250_ACCELRANGE_RESET						0b11100111
#define MPU9250_ACCELRANGE_MASK							0b00011000
#define MPU9250_ACCELHPF_RESET							0b11111000
#define MPU9250_ACCELHPF_MASK							0b00000111
#define MPU9250_FSYNC_RESET								0b11000111
#define MPU9250_FSYNC_MASK								0b00111000
#define MPU9250_DLPF_RESET 								0b11111000
#define MPU9250_DLPF_MASK 								0b00000111
#define MPU9250_SLEEP_RESET								0b10011111
#define MPU9250_SLEEP_MASK								0b01100000
#define MPU9250_CYCLE_RESET								0b00111111
#define MPU9250_CYCLE_MASK								0b11000000

#endif