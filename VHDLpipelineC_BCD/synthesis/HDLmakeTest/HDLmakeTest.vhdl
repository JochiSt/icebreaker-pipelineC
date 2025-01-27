LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE ieee.numeric_std.ALL;

ENTITY hdlmaketest IS
	GENERIC (
		clock_frequency : INTEGER := 12
	);
	PORT (
		CLK : IN STD_LOGIC;

		TX : OUT STD_LOGIC; -- UART TX out
		RX : IN STD_LOGIC;  -- UART RX in

		BTN_N : IN STD_LOGIC; -- RESET button

		LEDR_N : OUT STD_LOGIC; -- Red LED
		LEDG_N : OUT STD_LOGIC; -- Green LED

		P1A1  : OUT STD_LOGIC; -- PMOD1A (7-segment) 
		P1A2  : OUT STD_LOGIC;
		P1A3  : OUT STD_LOGIC;
		P1A4  : OUT STD_LOGIC;
		P1A7  : OUT STD_LOGIC;
		P1A8  : OUT STD_LOGIC;
		P1A9  : OUT STD_LOGIC;
		P1A10 : OUT STD_LOGIC;

		P1B1  : IN STD_LOGIC; -- PMOD1B (8 Switches)
		P1B2  : IN STD_LOGIC;
		P1B3  : IN STD_LOGIC;
		P1B4  : IN STD_LOGIC;
		P1B7  : IN STD_LOGIC;
		P1B8  : IN STD_LOGIC;
		P1B9  : IN STD_LOGIC;
		P1B10 : IN STD_LOGIC;

		LED1 : OUT STD_LOGIC; -- PMOD2 middle LED 
		LED2 : OUT STD_LOGIC; -- PMOD2 left LED 
		LED3 : OUT STD_LOGIC; -- PMOD2 right LED 
		LED4 : OUT STD_LOGIC; -- PMOD2 top LED 
		LED5 : OUT STD_LOGIC; -- PMOD2 bottom LED 
		BTN1 : IN STD_LOGIC;  -- PMOD2 BTN 1
		BTN2 : IN STD_LOGIC;  -- PMOD2 BTN 2
		BTN3 : IN STD_LOGIC   -- PMOD2 BTN 3
	);
END bcd_top;

ARCHITECTURE behaviour OF hdlmaketest IS

	SIGNAL dip_switches : STD_LOGIC_VECTOR(7 DOWNTO 0) := (OTHERS => '0');

BEGIN
	----------------------------------------------------------------------------
	-- assign and map IOs to switches and LED
	dip_switches <= P1B10 & P1B9 & P1B8 & P1B7 & P1B4 & P1B3 & P1B2 & P1B1;

	-- assign segments to outputs
	P1A9 <= dip_switches(6);
	P1A8 <= dip_switches(5);
	P1A7 <= dip_switches(4);
	P1A4 <= dip_switches(3);
	P1A3 <= dip_switches(2);
	P1A2 <= dip_switches(1);
	P1A1 <= dip_switches(0);

END behaviour; -- arch
