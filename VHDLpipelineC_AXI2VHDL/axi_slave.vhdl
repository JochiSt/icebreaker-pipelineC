LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE IEEE.NUMERIC_STD.ALL;

ENTITY axi_slave IS
    GENERIC (
        FIFO_SIZE : INTEGER := 32
    );
    PORT (
        ACLK       : IN STD_LOGIC;                     -- global clock
        ARESETn    : IN STD_LOGIC;                     -- reset signal belonging to ACLK

        TDATA_RXD  : IN STD_LOGIC_VECTOR(31 DOWNTO 0); -- receiving data
        TREADY_RXD : OUT STD_LOGIC;                    -- mark that we are ready for new data
        TVALID_RXD : IN STD_LOGIC;                     -- master signaling valid data
        TLAST_RXD  : IN STD_LOGIC;                     -- master signaling last data

        Index      : IN STD_LOGIC_VECTOR(4 DOWNTO 0);  -- data selection
        DataOut    : OUT STD_LOGIC_VECTOR(31 DOWNTO 0) -- selected data
    );
END axi_slave;

ARCHITECTURE behavioral OF axi_slave IS

    TYPE State_t IS (Reset, Ready, WaitForValid);
    TYPE FIFO_t IS ARRAY(0 TO (FIFO_SIZE - 1)) OF STD_LOGIC_VECTOR(31 DOWNTO 0);

    SIGNAL NextState    : State_t := Reset;

    SIGNAL FIFO         : FIFO_t  := (OTHERS => (OTHERS => '0'));
    SIGNAL FIFO_Counter : INTEGER := 0;

BEGIN

    PROCESS
    BEGIN
        WAIT UNTIL rising_edge(ACLK);

        IF (ARESETn = '0') THEN
            NextState <= Reset;
        ELSE
            CASE NextState IS
                WHEN Reset =>
                    FIFO         <= (OTHERS => (OTHERS => '0'));
                    FIFO_Counter <= 0;
                    NextState    <= Ready;

                WHEN Ready =>
                    TREADY_RXD <= '1';
                    NextState  <= WaitForValid;

                WHEN WaitForValid =>
                    IF (TVALID_RXD = '1') THEN
                        TREADY_RXD         <= '0';
                        FIFO(FIFO_Counter) <= TDATA_RXD;

                        IF ((FIFO_Counter = (FIFO_SIZE - 1)) OR (TLAST_RXD = '1')) THEN
                            FIFO_Counter <= 0;
                        ELSE
                            FIFO_Counter <= FIFO_Counter + 1;
                        END IF;

                        NextState <= Ready;
                    ELSE
                        NextState <= WaitForValid;
                    END IF;

            END CASE;
        END IF;
    END PROCESS;

    DataOut <= FIFO(to_integer(UNSIGNED(Index)));
END behavioral;
