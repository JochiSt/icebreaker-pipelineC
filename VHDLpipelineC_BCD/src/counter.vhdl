LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE ieee.numeric_std.ALL;

ENTITY counter IS
    GENERIC (
        BITWIDTH : INTEGER RANGE 0 TO 16 := 1
    );
    PORT (
        clock     : IN STD_LOGIC;
        enable    : IN STD_LOGIC;
        direction : IN STD_LOGIC;
        count     : OUT STD_LOGIC_VECTOR(BITWIDTH - 1 DOWNTO 0)
    );

END counter;

ARCHITECTURE behaviour OF counter IS
    SIGNAL counts : unsigned(BITWIDTH - 1 DOWNTO 0) := (OTHERS => '0');
BEGIN

    proc_cnt : PROCESS (clock)
    BEGIN
        IF rising_edge(clock) THEN
            IF enable = '1' THEN
                IF direction = '0' THEN
                    counts <= counts + 1;
                ELSE
                    counts <= counts - 1;
                END IF;

            END IF;
        END IF;

    END PROCESS; -- proc_cnt

    count <= std_logic_vector(counts);    -- propagate the signal to output

END ARCHITECTURE; -- arch
