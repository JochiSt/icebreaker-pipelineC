lIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.NUMERIC_STD.ALL;

ENTITY main IS
PORT (
    CLK  : IN  std_logic;   -- 12 MHz
    LED1 : OUT std_logic;
    LED2 : OUT std_logic;
    LED3 : OUT std_logic;
    LED4 : OUT std_logic;
    LED5 : OUT std_logic
    );
END main;


ARCHITECTURE behaviour of main is
    SIGNAL counter : UNSIGNED(24 downto 0);
    SIGNAL sled5 : std_logic := '0';
BEGIN

    P1 : PROCESS( clk ) IS
    BEGIN
        if rising_edge(clk) then
            counter <= counter + 1;
            if (to_integer(counter) = 24000000) then
                counter <= (others=>'0');
            end if;
        end if;
    END PROCESS P1;

    Pled1 : PROCESS(clk) IS
    BEGIN
        if rising_edge(clk) then
            if (to_integer(counter) = 12000000) or (counter = 0) then
                sled5 <= not sled5;
            end if;
        end if;
    END PROCESS Pled1;

    led1 <= counter(6);
    led2 <= counter(7);

    led3 <= counter(12);
    led4 <= counter(13);

    led5 <= sled5;

END behaviour;


