LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE ieee.numeric_std.ALL;

ENTITY bcd_top IS
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

		P1B1  : IN STD_LOGIC; --	PMOD1B (8 Switches)
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

ARCHITECTURE behaviour OF bcd_top IS

	SIGNAL dip_switches : STD_LOGIC_VECTOR(7 DOWNTO 0) := (OTHERS => '0');

	COMPONENT counter IS
		GENERIC (
			BITWIDTH : INTEGER RANGE 0 TO 16 := 1
		);
		PORT (
			clock     : IN STD_LOGIC;
			enable    : IN STD_LOGIC;
			direction : IN STD_LOGIC;
			count     : OUT STD_LOGIC_VECTOR(BITWIDTH - 1 DOWNTO 0)
		);

	END COMPONENT;

	COMPONENT mux IS
		PORT (
			clk_100p0 : IN STD_LOGIC;

			-- IO for each main func
			mux_sel           : IN unsigned(0 DOWNTO 0);
			mux_A             : IN unsigned(3 DOWNTO 0);
			mux_B             : IN unsigned(3 DOWNTO 0);
			mux_return_output : OUT unsigned(3 DOWNTO 0)
		);
	END COMPONENT;
	SIGNAL mux_sel : unsigned(0 DOWNTO 0) := (OTHERS => '0'); -- identical to mux_sel_slv
	SIGNAL mux_sel_slv : STD_LOGIC_VECTOR(0 DOWNTO 0) := (OTHERS => '0'); -- identical to mux_sel

	SIGNAL mux_digit : unsigned(3 DOWNTO 0) := (OTHERS => '0');

	COMPONENT LUT_7segment IS
		PORT (
			clk_in       : IN STD_LOGIC;
			decimal_in   : IN STD_LOGIC_VECTOR(3 DOWNTO 0);
			segments_out : OUT STD_LOGIC_VECTOR(6 DOWNTO 0)
		);
	END COMPONENT;

	SIGNAL segments : STD_LOGIC_VECTOR(6 DOWNTO 0) := (OTHERS => '0');

	COMPONENT led_blink_counter IS
		PORT (
			clk_12p0                        : IN STD_LOGIC;
			led_blink_counter_return_output : OUT unsigned(0 DOWNTO 0)
		);
	END COMPONENT;
	SIGNAL led_blink_counter_return_output : unsigned(0 DOWNTO 0);

BEGIN

	led_blink_counter_0 : led_blink_counter
	PORT MAP(
		clk_12p0                        => CLK,
		led_blink_counter_return_output => led_blink_counter_return_output
	);
	LEDG_N <= std_logic(led_blink_counter_return_output(0));

	digit_counter : counter
	GENERIC MAP(
		BITWIDTH => 1
	)
	PORT MAP(
		clock     => CLK,
		enable    => '1',
		direction => '0',
		count     => mux_sel_slv
	);
	mux_sel <= unsigned(mux_sel_slv);

	digit_mux : mux
	PORT MAP(
		clk_100p0 => CLK,

		-- IO for each main func
		mux_sel           => mux_sel,
		mux_A => (OTHERS => '1'),
		mux_B => (OTHERS => '0'),
		mux_return_output => mux_digit
	);

	LUT_7segment_0 : LUT_7segment
	PORT MAP(
		clk_in       => CLK,
		decimal_in   => STD_LOGIC_VECTOR(mux_digit),
		segments_out => segments
	);

	dip_switches <= P1B10 & P1B9 & P1B8 & P1B7 & P1B4 & P1B3 & P1B2 & P1B1;

	-- assign segments to outputs
	P1A9 <= segments(6);
	P1A8 <= segments(5);
	P1A7 <= segments(4);
	P1A4 <= segments(3);
	P1A3 <= segments(2);
	P1A2 <= segments(1);
	P1A1 <= segments(0);
	-- assign anode
	P1A10 <= mux_sel_slv(0); -- toggle between 0 and 1

	LEDR_N <= mux_sel_slv(0);

END behaviour; -- arch
