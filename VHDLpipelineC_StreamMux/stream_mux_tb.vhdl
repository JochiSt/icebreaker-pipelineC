LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE IEEE.NUMERIC_STD.ALL;

LIBRARY work;

ENTITY stream_mux_tb IS
END ENTITY stream_mux_tb;

ARCHITECTURE rtl OF stream_mux_tb IS
    SIGNAL clk_100p0 : STD_LOGIC := '0';

    SIGNAL trigger : STD_LOGIC := '0';

    SIGNAL TDATA_RXD  : STD_LOGIC_VECTOR(31 DOWNTO 0); -- receiving data
    SIGNAL TREADY_RXD : STD_LOGIC;                     -- mark that we are ready for new data
    SIGNAL TVALID_RXD : STD_LOGIC;                     -- master signaling valid data
    SIGNAL TLAST_RXD  : STD_LOGIC;                     -- master signaling last data

    SIGNAL uTDATA_RXD  : UNSIGNED(31 DOWNTO 0); -- receiving data
    SIGNAL uTREADY_RXD : UNSIGNED(0 DOWNTO 0);                     -- mark that we are ready for new data
    SIGNAL uTVALID_RXD : UNSIGNED(0 DOWNTO 0);                     -- master signaling valid data
    SIGNAL uTLAST_RXD  : UNSIGNED(0 DOWNTO 0);                     -- master signaling last data

    ----------------------------------------------------------------------------
    SIGNAL TDATA_TXD  : STD_LOGIC_VECTOR(31 DOWNTO 0); -- data, which is transmitted
    SIGNAL TREADY_TXD : STD_LOGIC;                     -- is slave ready for new data
    SIGNAL TVALID_TXD : STD_LOGIC;                     -- master signaling valid data
    SIGNAL TLAST_TXD  : STD_LOGIC;                     -- last data of packet

    SIGNAL uTDATA_TXD  : UNSIGNED(31 DOWNTO 0); -- data, which is transmitted
    SIGNAL uTREADY_TXD : UNSIGNED(0 DOWNTO 0);  -- is slave ready for new data
    SIGNAL uTVALID_TXD : UNSIGNED(0 DOWNTO 0);  -- master signaling valid data
    SIGNAL uTLAST_TXD  : UNSIGNED(0 DOWNTO 0);  -- last data of packet

BEGIN

    pipelinec_top_0 : ENTITY work.pipelinec_top
        PORT MAP(
            -- All clocks
            clk_100p0 => clk_100p0,

            -- slave axi receive data from other module
            s_axis_tdata  => uTDATA_TXD,
            s_axis_tkeep  => "0000",
            s_axis_tlast  => uTLAST_TXD,
            s_axis_tvalid => uTVALID_TXD,
            s_axis_tready => uTREADY_TXD,

            -- master axi send data to other module
            m_axis0_tdata  => uTDATA_RXD,
            m_axis0_tkeep  => OPEN,
            m_axis0_tlast  => uTLAST_RXD,
            m_axis0_tvalid => uTVALID_RXD,
            m_axis0_tready => uTREADY_RXD,

            m_axis1_tdata  => OPEN,
            m_axis1_tkeep  => OPEN,
            m_axis1_tlast  => OPEN,
            m_axis1_tvalid => OPEN,
            m_axis1_tready => uTREADY_RXD
        );

    axi_slave_0 : ENTITY work.axi_slave
        GENERIC MAP(
            FIFO_SIZE => 32
        )
        PORT MAP(
            ACLK    => clk_100p0,
            ARESETn => '1',

            TDATA_RXD  => TDATA_RXD,
            TREADY_RXD => TREADY_RXD,
            TVALID_RXD => TVALID_RXD,
            TLAST_RXD  => TLAST_RXD,

            Index   => "00000",
            DataOut => OPEN
        );

    axi_master_0 : ENTITY work.axi_master
        GENERIC MAP(
            LENGTH => 100
        )
        PORT MAP(
            ACLK    => clk_100p0,
            ARESETn => '1',

            Trigger => trigger,

            TDATA_TXD  => TDATA_TXD,
            TREADY_TXD => TREADY_TXD,
            TVALID_TXD => TVALID_TXD,
            TLAST_TXD  => TLAST_TXD
        );

    ----------------------------------------------------------------------------
    uTDATA_TXD <= UNSIGNED(TDATA_TXD);
    TREADY_TXD <= STD_LOGIC(uTREADY_TXD(0));
    uTVALID_TXD(0) <= TVALID_TXD;

    TDATA_RXD <= STD_LOGIC_VECTOR(uTDATA_RXD);
    uTREADY_RXD(0) <= TREADY_RXD;
    TVALID_RXD <= STD_LOGIC(uTVALID_TXD(0));

    ----------------------------------------------------------------------------
    p_clk100MHz : PROCESS
    BEGIN
        WAIT FOR 5 ns;
        clk_100p0 <= NOT clk_100p0;
    END PROCESS;

    p_trigger : PROCESS
    BEGIN
        WAIT FOR 50 ns;
        trigger <= NOT trigger;
    END PROCESS;
END ARCHITECTURE;
