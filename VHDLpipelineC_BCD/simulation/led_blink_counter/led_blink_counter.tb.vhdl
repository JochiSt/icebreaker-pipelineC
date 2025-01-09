-- simulation of the top module

LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE ieee.numeric_std.ALL;

ENTITY led_blink_counter_tb IS

END led_blink_counter_tb;

ARCHITECTURE simulation OF led_blink_counter_tb IS
    COMPONENT led_blink_counter IS
        PORT (
            clk_12p0 : IN STD_LOGIC;
            -- IO for each main func
            led_blink_counter_return_output : OUT unsigned(0 DOWNTO 0)
        );
    END COMPONENT;

    SIGNAL CLK : STD_LOGIC := '0';

BEGIN

    led_blink_counter_0 : led_blink_counter
    PORT MAP(
        clk_12p0                        => CLK,
        led_blink_counter_return_output => OPEN
    );

    ----------------------------------------------------------------------------
    clk_proc : PROCESS
    BEGIN
        -- 83.3333 ns / 2 = 41.66666 ns
        CLK <= '0';
        WAIT FOR 41.66666 ns;
        CLK <= '1';
        WAIT FOR 41.66666 ns;
    END PROCESS; -- clk_proc
    ----------------------------------------------------------------------------
END ARCHITECTURE; -- arch
