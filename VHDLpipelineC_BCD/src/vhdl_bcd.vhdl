LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.numeric_std.ALL;

ENTITY vhdl_bcd IS
    PORT (
        i_slv_binary  : IN STD_LOGIC_VECTOR(4 DOWNTO 0);
        o_slv_decimal : OUT STD_LOGIC_VECTOR(2 * 4 - 1 DOWNTO 0)
    );
END vhdl_bcd;

----------------------------------------

ARCHITECTURE behavior OF vhdl_bcd IS
BEGIN
    PROCESS (i_slv_binary)
        -- temporarly variables
        VARIABLE tens : unsigned (3 DOWNTO 0) := (OTHERS => '0');
        VARIABLE ones : unsigned (3 DOWNTO 0) := (OTHERS => '0');
    BEGIN
        -- reset all outputs
        tens := (OTHERS => '0');
        ones := (OTHERS => '0');

        FOR i IN 3 DOWNTO 0 LOOP

            IF (tens >= 5) THEN
                tens := tens + 3;
            END IF;
            IF (ones >= 5) THEN
                ones := ones + 3;
            END IF;

            tens := tens(2 DOWNTO 0) & ones(3);
            ones := ones(2 DOWNTO 0) & i_slv_binary(i);

        END LOOP;

        o_slv_decimal <= STD_LOGIC_VECTOR(tens) & STD_LOGIC_VECTOR(ones);

    END PROCESS;
END behavior;

------------------------------------------
