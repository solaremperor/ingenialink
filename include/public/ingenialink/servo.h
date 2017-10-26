/*
 * MIT License
 *
 * Copyright (c) 2017 Ingenia-CAT S.L.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef PUBLIC_INGENIALINK_SERVO_H_
#define PUBLIC_INGENIALINK_SERVO_H_

#include "net.h"
#include "registers.h"

IL_BEGIN_DECL

/**
 * @file ingenialink/servo.h
 * @brief Servo.
 * @defgroup IL_SERVO Servo
 * @ingroup IL
 * @{
 */

/** IngeniaLink servo instance. */
typedef struct il_servo il_servo_t;

/** Default communications timeout (ms). */
#define IL_SERVO_TIMEOUT_DEF	100

/** Emergency subscriber callback. */
typedef void (*il_servo_emcy_subscriber_cb_t)(void *ctx, uint32_t code);

/** Servo states (CiA 402 PDS states). */
typedef enum {
	/** Not ready to switch on. */
	IL_SERVO_STATE_NRDY,
	/** Switch on disabled. */
	IL_SERVO_STATE_DISABLED,
	/** Ready to be switched on. */
	IL_SERVO_STATE_RDY,
	/** Power switched on. */
	IL_SERVO_STATE_ON,
	/** Enabled. */
	IL_SERVO_STATE_ENABLED,
	/** Quick stop. */
	IL_SERVO_STATE_QSTOP,
	/** Fault reactive. */
	IL_SERVO_STATE_FAULTR,
	/** Fault. */
	IL_SERVO_STATE_FAULT
} il_servo_state_t;

/** Servo operation modes. */
typedef enum {
	/** Open loop (vector mode). */
	IL_SERVO_MODE_OLV = -2,
	/** Open loop (scalar mode). */
	IL_SERVO_MODE_OLS = -1,
	/** Profile position. */
	IL_SERVO_MODE_PP = 1,
	/** Profile velocity. */
	IL_SERVO_MODE_PV = 3,
	/** Profile torque. */
	IL_SERVO_MODE_PT = 4,
	/** Homing. */
	IL_SERVO_MODE_HOMING = 6
} il_servo_mode_t;

/** Torque units. */
typedef enum {
	/** Native. */
	IL_UNITS_TORQUE_NATIVE,
	/** Millinewtons. */
	IL_UNITS_TORQUE_MN,
	/** Newtons. */
	IL_UNITS_TORQUE_N
} il_units_torque_t;

/** Position units. */
typedef enum {
	/** Native. */
	IL_UNITS_POS_NATIVE,
	/** Revolutions. */
	IL_UNITS_POS_REV,
	/** Radians. */
	IL_UNITS_POS_RAD,
	/** Degrees. */
	IL_UNITS_POS_DEG,
	/** Micrometers. */
	IL_UNITS_POS_UM,
	/** Millimeters. */
	IL_UNITS_POS_MM,
	/** Meters. */
	IL_UNITS_POS_M,
} il_units_pos_t;

/** Velocity units. */
typedef enum {
	/** Native. */
	IL_UNITS_VEL_NATIVE,
	/** Revolutions per second. */
	IL_UNITS_VEL_RPS,
	/** Revolutions per minute. */
	IL_UNITS_VEL_RPM,
	/** Radians/second. */
	IL_UNITS_VEL_RAD_S,
	/** Degrees/second. */
	IL_UNITS_VEL_DEG_S,
	/** Micrometers/second. */
	IL_UNITS_VEL_UM_S,
	/** Millimeters/second. */
	IL_UNITS_VEL_MM_S,
	/** Meters/second. */
	IL_UNITS_VEL_M_S,
} il_units_vel_t;

/** Acceleration units. */
typedef enum {
	/** Native. */
	IL_UNITS_ACC_NATIVE,
	/** Revolutions/second^2. */
	IL_UNITS_ACC_REV_S2,
	/** Radians/second^2. */
	IL_UNITS_ACC_RAD_S2,
	/** Degrees/second^2. */
	IL_UNITS_ACC_DEG_S2,
	/** Micrometers/second^2. */
	IL_UNITS_ACC_UM_S2,
	/** Millimeters/second^2. */
	IL_UNITS_ACC_MM_S2,
	/** Meters/second^2. */
	IL_UNITS_ACC_M_S2,
} il_units_acc_t;

/**
 * Create IngeniaLink servo instance.
 *
 * @param [in] net
 *	IngeniaLink network.
 * @param [in] id
 *	Servo id.
 * @param [in] timeout
 *	Communications timeout (ms).
 *
 * @return
 *	Servo instance (NULL if it could not be created).
 */
IL_EXPORT il_servo_t *il_servo_create(il_net_t *net, uint8_t id, int timeout);

/**
 * Destroy an IngeniaLink servo instance.
 *
 * @param [in] servo
 *	IngeniaLink servo instance.
 */
IL_EXPORT void il_servo_destroy(il_servo_t *servo);

/**
 * Subscribe to emergency messages.
 *
 * @param [in] servo
 *	IngeniaLink servo.
 * @param [in] cb
 *	Callback.
 * @param [in] ctx
 *	Callback context.
 *
 * @returns
 *	Assigned slot (>= 0) or error code (< 0).
 */
IL_EXPORT int il_servo_emcy_subscribe(il_servo_t *servo,
				      il_servo_emcy_subscriber_cb_t cb,
				      void *ctx);

/**
 * Unubscribe from emergency messages.
 *
 * @param [in] servo
 *	IngeniaLink servo.
 * @param [in] slot
 *	Assigned slot when subscribed.
 */
IL_EXPORT void il_servo_emcy_unsubscribe(il_servo_t *servo, int slot);

/**
 * Update units scaling factors.
 *
 * @note
 *	This must be called if any encoder parameter, rated torque or pole pitch
 *	are changed, otherwise, the readings conversions will not be correct.
 *
 * @param [in] servo
 *	IngeniaLink servo.
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_servo_units_update(il_servo_t *servo);

/**
 * Obtain the units scale factor associated with the given register.
 *
 * @param [in] servo
 *	IngeniaLink servo.
 * @param [in] reg
 *	Register.
 *
 * @return
 *	Scale factor.
 */
IL_EXPORT double il_servo_units_factor(il_servo_t *servo, const il_reg_t *reg);

/**
 * Get the torque units.
 *
 * @param [in] servo
 *	IngeniaLink servo.
 *
 * @return
 *	Torque units (IL_UNITS_TORQUE_NATIVE if servo is not valid).
 */
IL_EXPORT il_units_torque_t il_servo_units_torque_get(il_servo_t *servo);

/**
 * Set the torque units.
 *
 * @param [in] servo
 *	IngeniaLink servo.
 * @param [in] units
 *	Units.
 */
IL_EXPORT void il_servo_units_torque_set(il_servo_t *servo,
					 il_units_torque_t units);

/**
 * Get the position units.
 *
 * @param [in] servo
 *	IngeniaLink servo.
 *
 * @return
 *	Position units (IL_UNITS_POS_NATIVE if servo is not valid).
 */
IL_EXPORT il_units_pos_t il_servo_units_pos_get(il_servo_t *servo);

/**
 * Set the position units.
 *
 * @param [in] servo
 *	IngeniaLink servo.
 * @param [in] units
 *	Units.
 */
IL_EXPORT void il_servo_units_pos_set(il_servo_t *servo, il_units_pos_t units);

/**
 * Get the velocity units.
 *
 * @param [in] servo
 *	IngeniaLink servo.
 *
 * @return
 *	Velocity units (IL_UNITS_VEL_NATIVE if servo is not valid).
 */
IL_EXPORT il_units_vel_t il_servo_units_vel_get(il_servo_t *servo);

/**
 * Set the velocity units.
 *
 * @param [in] servo
 *	IngeniaLink servo.
 * @param [in] units
 *	Units.
 */
IL_EXPORT void il_servo_units_vel_set(il_servo_t *servo, il_units_vel_t units);

/**
 * Get the acceleration units.
 *
 * @param [in] servo
 *	IngeniaLink servo.
 *
 * @return
 *	Acceleration units (IL_UNITS_ACC_NATIVE if servo is not valid).
 */
IL_EXPORT il_units_acc_t il_servo_units_acc_get(il_servo_t *servo);

/**
 * Set the acceleration units.
 *
 * @param [in] servo
 *	IngeniaLink servo.
 * @param [in] units
 *	Units.
 */
IL_EXPORT void il_servo_units_acc_set(il_servo_t *servo, il_units_acc_t units);

/**
 * Read data from a register.
 *
 * @param [in] servo
 *	IngeniaLink servo.
 * @param [in] reg
 *	Register.
 * @param [out] buf
 *	Buffer where to store received data.
 * @param [in] sz
 *	Buffer size.
 * @param [out] recvd
 *	Variable where the number of bytes received will be stored (optional).
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_servo_raw_read(il_servo_t *servo, const il_reg_t *reg,
				void *buf, size_t sz, size_t *recvd);

/**
 * Read unsigned 8-bit value from a register.
 *
 * @param [in] servo
 *	IngeniaLink servo.
 * @param [in] reg
 *	Register.
 * @param [out] buf
 *	Buffer where to store received data.
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_servo_raw_read_u8(il_servo_t *servo, const il_reg_t *reg,
				   uint8_t *buf);

/**
 * Read signed 8-bit value from a register.
 *
 * @param [in] servo
 *	IngeniaLink servo.
 * @param [in] reg
 *	Register.
 * @param [out] buf
 *	Buffer where to store received data.
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_servo_raw_read_s8(il_servo_t *servo, const il_reg_t *reg,
				   int8_t *buf);

/**
 * Read unsigned 16-bit value from a register.
 *
 * @param [in] servo
 *	IngeniaLink servo.
 * @param [in] reg
 *	Register.
 * @param [out] buf
 *	Buffer where to store received data.
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_servo_raw_read_u16(il_servo_t *servo, const il_reg_t *reg,
				    uint16_t *buf);

/**
 * Read signed 16-bit value from a register.
 *
 * @param [in] servo
 *	IngeniaLink servo.
 * @param [in] reg
 *	Register.
 * @param [out] buf
 *	Buffer where to store received data.
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_servo_raw_read_s16(il_servo_t *servo, const il_reg_t *reg,
				    int16_t *buf);

/**
 * Read unsigned 32-bit value from a register.
 *
 * @param [in] servo
 *	IngeniaLink servo.
 * @param [in] reg
 *	Register.
 * @param [out] buf
 *	Buffer where to store received data.
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_servo_raw_read_u32(il_servo_t *servo, const il_reg_t *reg,
				    uint32_t *buf);

/**
 * Read signed 32-bit value from a register.
 *
 * @param [in] servo
 *	IngeniaLink servo.
 * @param [in] reg
 *	Register.
 * @param [out] buf
 *	Buffer where to store received data.
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_servo_raw_read_s32(il_servo_t *servo, const il_reg_t *reg,
				    int32_t *buf);

/**
 * Read unsigned 64-bit value from a register.
 *
 * @param [in] servo
 *	IngeniaLink servo.
 * @param [in] reg
 *	Register.
 * @param [out] buf
 *	Buffer where to store received data.
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_servo_raw_read_u64(il_servo_t *servo, const il_reg_t *reg,
				    uint64_t *buf);

/**
 * Read signed 64-bit value from a register.
 *
 * @param [in] servo
 *	IngeniaLink servo.
 * @param [in] reg
 *	Register.
 * @param [out] buf
 *	Buffer where to store received data.
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_servo_raw_read_s64(il_servo_t *servo, const il_reg_t *reg,
				    int64_t *buf);

/**
 * Read a register.
 *
 * This function will read from a register and also perform a unit conversion to
 * match the current operating units.
 *
 * @param [in] servo
 *	IngeniaLink servo.
 * @param [in] reg
 *	Register.
 * @param [out] buf
 *	Buffer where adjusted register content will be stored.
 *
 * @returns
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_servo_read(il_servo_t *servo, const il_reg_t *reg,
			    double *buf);

/**
 * Write data to a register.
 *
 * @param [in] servo
 *	IngeniaLink servo.
 * @param [in] reg
 *	Register.
 * @param [in] buf
 *	Buffer where to pick data from.
 * @param [in] sz
 *	Buffer size.
 * @param [in] confirm
 *	Confirm the write.
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_servo_raw_write(il_servo_t *servo, const il_reg_t *reg,
				 const void *buf, size_t sz, int confirm);

/**
 * Write unsigned 8-bit integer to a register.
 *
 * @param [in] servo
 *	IngeniaLink servo.
 * @param [in] reg
 *	Register.
 * @param [in] val
 *	Value.
 * @param [in] confirm
 *	Confirm the write.
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_servo_raw_write_u8(il_servo_t *servo, const il_reg_t *reg,
				    uint8_t val, int confirm);

/**
 * Write signed 8-bit integer to a register.
 *
 * @param [in] servo
 *	IngeniaLink servo.
 * @param [in] reg
 *	Register.
 * @param [in] val
 *	Value.
 * @param [in] confirm
 *	Confirm the write.
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_servo_raw_write_s8(il_servo_t *servo, const il_reg_t *reg,
				    int8_t val, int confirm);

/**
 * Write unsigned 16-bit integer to a register.
 *
 * @param [in] servo
 *	IngeniaLink servo.
 * @param [in] reg
 *	Register.
 * @param [in] val
 *	Value.
 * @param [in] confirm
 *	Confirm the write.
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_servo_raw_write_u16(il_servo_t *servo, const il_reg_t *reg,
				     uint16_t val, int confirm);

/**
 * Write signed 16-bit integer to a register.
 *
 * @param [in] servo
 *	IngeniaLink servo.
 * @param [in] reg
 *	Register.
 * @param [in] val
 *	Value.
 * @param [in] confirm
 *	Confirm the write.
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_servo_raw_write_s16(il_servo_t *servo, const il_reg_t *reg,
				     int16_t val, int confirm);

/**
 * Write unsigned 32-bit integer to a register.
 *
 * @param [in] servo
 *	IngeniaLink servo.
 * @param [in] reg
 *	Register.
 * @param [in] val
 *	Value.
 * @param [in] confirm
 *	Confirm the write.
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_servo_raw_write_u32(il_servo_t *servo, const il_reg_t *reg,
				     uint32_t val, int confirm);

/**
 * Write signed 32-bit integer to a register.
 *
 * @param [in] servo
 *	IngeniaLink servo.
 * @param [in] reg
 *	Register.
 * @param [in] val
 *	Value.
 * @param [in] confirm
 *	Confirm the write.
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_servo_raw_write_s32(il_servo_t *servo, const il_reg_t *reg,
				     int32_t val, int confirm);

/**
 * Write unsigned 64-bit integer to a register.
 *
 * @param [in] servo
 *	IngeniaLink servo.
 * @param [in] reg
 *	Register.
 * @param [in] val
 *	Value.
 * @param [in] confirm
 *	Confirm the write.
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_servo_raw_write_u64(il_servo_t *servo, const il_reg_t *reg,
				     uint64_t val, int confirm);

/**
 * Write signed 64-bit integer to a register.
 *
 * @param [in] servo
 *	IngeniaLink servo.
 * @param [in] reg
 *	Register.
 * @param [in] val
 *	Value.
 * @param [in] confirm
 *	Confirm the write.
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_servo_raw_write_s64(il_servo_t *servo, const il_reg_t *reg,
				     int64_t val, int confirm);

/**
 * Write to a register.
 *
 * This function will write to a register and also perform a unit conversion to
 * match the current operating units.
 *
 * @param [in] servo
 *	IngeniaLink servo.
 * @param [in] reg
 *	Register.
 * @param [in] val
 *	Value.
 * @param [in] confirm
 *	Confirm the write.
 *
 * @returns
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_servo_write(il_servo_t *servo, const il_reg_t *reg,
			     double val, int confirm);

/**
 * Obtain current servo PDS state.
 *
 * @param [in] servo
 *	IngeniaLink servo.
 *
 * @return
 *	Current PDS state.
 */
il_servo_state_t il_servo_state_get(il_servo_t *servo);

/**
 * Disable servo PDS.
 *
 * @param [in] servo
 *	IngeniaLink servo.
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_servo_disable(il_servo_t *servo);

/**
 * Switch on servo PDS.
 *
 * @note
 *	The timeout is the timeout between state changes.
 *
 * @param [in] servo
 *	IngeniaLink servo.
 * @param [in] timeout
 *	Timeout (ms).
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_servo_switch_on(il_servo_t *servo, int timeout);

/**
 * Enable servo PDS.
 *
 * @note
 *	The timeout is the timeout between state changes.
 *
 * @param [in] servo
 *	IngeniaLink servo.
 * @param [in] timeout
 *	Timeout (ms).
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_servo_enable(il_servo_t *servo, int timeout);

/**
 * Reset the drive fault state.
 *
 * @param [in] servo
 *	IngeniaLink servo.
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_servo_fault_reset(il_servo_t *servo);

/**
 * Set the servo operation mode.
 *
 * @param [in] servo
 *	IngeniaLink servo.
 * @param [in] mode
 *	Mode.
 *
 */
IL_EXPORT int il_servo_mode_set(il_servo_t *servo, il_servo_mode_t mode);

/**
 * Get the open loop voltage.
 *
 * @param [in] servo
 *	IngeniaLink servo.
 * @param [out] voltage
 *	Voltage buffer (% relative to DC-Bus, -1...1).
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_servo_ol_voltage_get(il_servo_t *servo, double *voltage);

/**
 * Set the open loop voltage.
 *
 * @param [in] servo
 *	IngeniaLink servo.
 * @param [in] voltage
 *	Voltage (% relative to DC-Bus, -1...1).
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_servo_ol_voltage_set(il_servo_t *servo, double voltage);

/**
 * Get the open loop frequency.
 *
 * @param [in] servo
 *	IngeniaLink servo.
 * @param [in] freq
 *	Frequency buffer (mHz).
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_servo_ol_frequency_get(il_servo_t *servo, double *freq);

/**
 * Set the open loop frequency.
 *
 * @param [in] servo
 *	IngeniaLink servo.
 * @param [in] freq
 *	Frequency (mHz).
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_servo_ol_frequency_set(il_servo_t *servo, double freq);

/**
 * Start homing.
 *
 * @note
 *	This assumes that the drive is in the appropriate mote and that the PDS
 *	is enabled.
 *
 * @param [in] servo
 *	IngeniaLink servo.
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_servo_homing_start(il_servo_t *servo);

/**
 * Wait until homing completes.
 *
 * @param [in] servo
 *	IngeniaLink servo.
 * @param [in] timeout
 *	Timeout (ms).
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_servo_homing_wait(il_servo_t *servo, int timeout);

/**
 * Get the actual servo torque.
 *
 * @param [in] servo
 *	IngeniaLink servo.
 * @param [out] torque
 *	Where the actual torque will be stored.
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_servo_torque_get(il_servo_t *servo, double *torque);

/**
 * Set the servo target torque.
 *
 * @param [in] servo
 *	IngeniaLink servo.
 * @param [in] torque
 *	Torque.
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_servo_torque_set(il_servo_t *servo, double torque);

/**
 * Get the actual servo position.
 *
 * @param [in] servo
 *	IngeniaLink servo.
 * @param [out] pos
 *	Where the actual position will be stored.
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_servo_position_get(il_servo_t *servo, double *pos);

/**
 * Set the servo target position.
 *
 * @param [in] servo
 *	IngeniaLink servo.
 * @param [in] pos
 *	Position.
 * @param [in] immediate
 *	If set, the position will be set immediately, otherwise will be added
 *	to the internal servo queue.
 * @param [in] relative
 *	If set, the position is taken as a relative value, otherwise it is taken
 *	as an absolute value.
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_servo_position_set(il_servo_t *servo, double pos,
				    int immediate, int relative);

/**
 * Wait until the servo acknowledges a position.
 *
 * @note
 *	This may only be useful for multi-point movements, where the ACK can be
 *	kept high until positions buffer is empty. On any other case, ACK will
 *	always be received.
 *
 * @param [in] servo
 *	IngeniaLink servo.
 * @param [in] timeout
 *	Timeout (ms).
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_servo_position_wait_ack(il_servo_t *servo, int timeout);

/**
 * Obtain position resolution.
 *
 * @param [in] servo
 *	IngeniaLink servo.
 * @param [out] res
 *	Position resolution (c/rev, c/ppitch).
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_servo_position_res_get(il_servo_t *servo, uint32_t *res);

/**
 * Get the actual servo velocity.
 *
 * @param [in] servo
 *	IngeniaLink servo.
 * @param [out] vel
 *	Where the actual velocity will be stored.
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_servo_velocity_get(il_servo_t *servo, double *vel);

/**
 * Set the servo target velocity.
 *
 * @param [in] servo
 *	IngeniaLink servo.
 * @param [in] vel
 *	Velocity.
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_servo_velocity_set(il_servo_t *servo, double vel);

/**
 * Obtain velocity resolution.
 *
 * @param [in] servo
 *	IngeniaLink servo.
 * @param [out] res
 *	Velocity resolution (c/rev/s).
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_servo_velocity_res_get(il_servo_t *servo, uint32_t *res);

/**
 * Wait until the servo does a target reach.
 *
 * @param [in] servo
 *	IngeniaLink servo.
 * @param [in] timeout
 *	Timeout (ms).
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_servo_wait_reached(il_servo_t *servo, int timeout);

/** @} */

IL_END_DECL

#endif
