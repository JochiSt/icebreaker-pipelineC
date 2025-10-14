-- Simulation TOP module
LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.numeric_std.ALL;

ENTITY sim_top IS
    PORT (
        CLK : IN STD_LOGIC;

        pll_clk    : IN unsigned(0 DOWNTO 0);
        pll_locked : IN unsigned(0 DOWNTO 0);

        LEDR_N : OUT unsigned(0 DOWNTO 0);
        LEDG_N : OUT unsigned(0 DOWNTO 0);

        LED_RED_N : OUT unsigned(0 DOWNTO 0);
        LED_GRN_N : OUT unsigned(0 DOWNTO 0);
        LED_BLU_N : OUT unsigned(0 DOWNTO 0);

        TX : OUT unsigned(0 DOWNTO 0);
        RX : IN unsigned(0 DOWNTO 0);

        P1A9 : IN unsigned(0 DOWNTO 0);
        P1A3 : IN unsigned(0 DOWNTO 0);
        --P1A8  : IN unsigned(0 DOWNTO 0);
        --P1A2  : IN unsigned(0 DOWNTO 0);
        P1A7  : OUT unsigned(0 DOWNTO 0);
        P1A1  : OUT unsigned(0 DOWNTO 0);
        P1A10 : OUT unsigned(0 DOWNTO 0)

    );
END ENTITY sim_top;

ARCHITECTURE rtl OF sim_top IS
    SIGNAL clk_None : STD_LOGIC := '0';

    SIGNAL eth_rx_vect     : UNSIGNED(1 DOWNTO 0);
    SIGNAL eth_rx_vect_std : STD_LOGIC_VECTOR(1 DOWNTO 0);
    SIGNAL eth_tx_vect     : UNSIGNED(1 DOWNTO 0);

    SIGNAL P1A2 : unsigned(0 DOWNTO 0);
    SIGNAL P1A8 : unsigned(0 DOWNTO 0);

BEGIN
    clk_None <= CLK;

    -- convert RX into single bits but keep the vectors
    eth_rx_vect_std <= STD_LOGIC_VECTOR(eth_rx_vect);
    P1A2            <= unsigned(eth_rx_vect_std(1 DOWNTO 1));
    P1A8            <= unsigned(eth_rx_vect_std(0 DOWNTO 0));
    -- join individual bits to the TX vector
    eth_tx_vect <= P1A10 & P1A1;

    pipelinec_top_0 : ENTITY work.pipelinec_top
        PORT MAP(
            -- All clocks
            clk_None => clk_None,

            -- Global wires marked as top level IO
            pll_clk    => pll_clk,
            pll_locked => pll_locked,

            LEDR_N    => LEDR_N,
            LEDG_N    => LEDG_N,
            LED_RED_N => LED_RED_N,
            LED_GRN_N => LED_GRN_N,
            LED_BLU_N => LED_BLU_N,

            TX => TX,
            RX => RX,

            P1A9  => P1A9,
            P1A3  => P1A3,
            P1A8  => P1A8,
            P1A2  => P1A2,
            P1A7  => P1A7,
            P1A1  => P1A1,
            P1A10 => P1A10
        );

END ARCHITECTURE;
