-- simulation of the top module

LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE ieee.numeric_std.ALL;

ENTITY bcd_top_tb IS

END bcd_top_tb;

ARCHITECTURE simulation OF bcd_top_tb IS

    COMPONENT bcd_top IS
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
    END COMPONENT;

    SIGNAL CLK : STD_LOGIC := '0';

    SIGNAL dip_switches : STD_LOGIC_VECTOR(7 DOWNTO 0) := (OTHERS => '0');

BEGIN

    bcd_top_0 : bcd_top
    GENERIC MAP(
        clock_frequency => 12
    )
    PORT MAP(
        CLK => CLK,

        TX => OPEN,
        RX => '0',

        BTN_N => '0',

        LEDR_N => OPEN,
        LEDG_N => OPEN,

        P1A1  => OPEN,
        P1A2  => OPEN,
        P1A3  => OPEN,
        P1A4  => OPEN,
        P1A7  => OPEN,
        P1A8  => OPEN,
        P1A9  => OPEN,
        P1A10 => OPEN,

        -- P1B10 & P1B9 & P1B8 & P1B7 & P1B4 & P1B3 & P1B2 & P1B1;
        P1B1  => dip_switches(0),
        P1B2  => dip_switches(1),
        P1B3  => dip_switches(2),
        P1B4  => dip_switches(3),
        P1B7  => dip_switches(4),
        P1B8  => dip_switches(5),
        P1B9  => dip_switches(6),
        P1B10 => dip_switches(7),

        LED1 => OPEN,
        LED2 => OPEN,
        LED3 => OPEN,
        LED4 => OPEN,
        LED5 => OPEN,
        BTN1 => '0',
        BTN2 => '0',
        BTN3 => '0'
    );

    ----------------------------------------------------------------------------
    clk_proc : PROCESS
    BEGIN
        CLK <= '0';
        WAIT FOR 10 ns;
        CLK <= '1';
        WAIT FOR 10 ns;
    END PROCESS; -- clk_proc
    ----------------------------------------------------------------------------

    dip_proc : process
    begin
        wait for 40 ns;
        dip_switches <= "00000010";
        wait for 40 ns;
        dip_switches <= "00100000";
        wait for 10 ms; -- wait basically until end of simulation
    end process;

END ARCHITECTURE; -- arch
