LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;

ENTITY LUT_7segment IS
    PORT (
        clk_in       : IN STD_LOGIC;
        decimal_in   : IN STD_LOGIC_VECTOR(3 DOWNTO 0); -- 0 -> F
        segments_out : OUT STD_LOGIC_VECTOR(6 DOWNTO 0) -- G -> A
    );
END LUT_7segment;

ARCHITECTURE behaviour OF LUT_7segment IS

BEGIN

    PROCESS (clk_in) BEGIN
        IF rising_edge(clk_in) THEN
            CASE decimal_in IS
                WHEN x"0" =>
                    segments_out <= "0111111";
                WHEN x"1" =>
                    segments_out <= "0000110";
                WHEN x"2" =>
                    segments_out <= "1011011";
                WHEN x"3" =>
                    segments_out <= "1001111";
                WHEN x"4" =>
                    segments_out <= "1100110";
                WHEN x"5" =>
                    segments_out <= "1101101";
                WHEN x"6" =>
                    segments_out <= "1111101";
                WHEN x"7" =>
                    segments_out <= "0000111";
                WHEN x"8" =>
                    segments_out <= "1111111";
                WHEN x"9" =>
                    segments_out <= "1101111";
                WHEN x"A" =>
                    segments_out <= "1110111";
                WHEN x"B" =>
                    segments_out <= "1111100";
                WHEN x"C" =>
                    segments_out <= "0111001";
                WHEN x"D" =>
                    segments_out <= "1011110";
                WHEN x"E" =>
                    segments_out <= "1111001";
                WHEN x"F" =>
                    segments_out <= "1110001";
                WHEN OTHERS =>
                    segments_out <= "1000000";
            END CASE;
        END IF;
    END PROCESS;

END ARCHITECTURE; -- behaviour
