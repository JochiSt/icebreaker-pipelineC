LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE IEEE.NUMERIC_STD.ALL;

LIBRARY work;

ENTITY stream_mux_tb IS
END ENTITY stream_mux_tb;

ARCHITECTURE rtl OF stream_mux_tb IS
    SIGNAL clk_100p0 : STD_LOGIC := '0';

    SIGNAL trigger : STD_LOGIC := '0';

    ----------------------------------------------------------------------------
    -- first AXI slave
    SIGNAL TDATA_RXD_0  : STD_LOGIC_VECTOR(31 DOWNTO 0); -- receiving data
    SIGNAL TREADY_RXD_0 : STD_LOGIC;                     -- mark that we are ready for new data
    SIGNAL TVALID_RXD_0 : STD_LOGIC;                     -- master signaling valid data
    SIGNAL TLAST_RXD_0  : STD_LOGIC;                     -- master signaling last data

    SIGNAL uTDATA_RXD_0  : UNSIGNED(31 DOWNTO 0); -- receiving data
    SIGNAL uTREADY_RXD_0 : UNSIGNED(0 DOWNTO 0);  -- mark that we are ready for new data
    SIGNAL uTVALID_RXD_0 : UNSIGNED(0 DOWNTO 0);  -- master signaling valid data
    SIGNAL uTLAST_RXD_0  : UNSIGNED(0 DOWNTO 0);  -- master signaling last data

    ----------------------------------------------------------------------------
    -- second AXI slave
    SIGNAL TDATA_RXD_1  : STD_LOGIC_VECTOR(31 DOWNTO 0); -- receiving data
    SIGNAL TREADY_RXD_1 : STD_LOGIC;                     -- mark that we are ready for new data
    SIGNAL TVALID_RXD_1 : STD_LOGIC;                     -- master signaling valid data
    SIGNAL TLAST_RXD_1  : STD_LOGIC;                     -- master signaling last data

    SIGNAL uTDATA_RXD_1  : UNSIGNED(31 DOWNTO 0); -- receiving data
    SIGNAL uTREADY_RXD_1 : UNSIGNED(0 DOWNTO 0);  -- mark that we are ready for new data
    SIGNAL uTVALID_RXD_1 : UNSIGNED(0 DOWNTO 0);  -- master signaling valid data
    SIGNAL uTLAST_RXD_1  : UNSIGNED(0 DOWNTO 0);  -- master signaling last data

    ----------------------------------------------------------------------------
    SIGNAL TDATA_TXD_0  : STD_LOGIC_VECTOR(31 DOWNTO 0); -- data, which is transmitted
    SIGNAL TREADY_TXD_0 : STD_LOGIC;                     -- is slave ready for new data
    SIGNAL TVALID_TXD_0 : STD_LOGIC;                     -- master signaling valid data
    SIGNAL TLAST_TXD_0  : STD_LOGIC;                     -- last data of packet

    SIGNAL uTDATA_TXD_0  : UNSIGNED(31 DOWNTO 0); -- data, which is transmitted
    SIGNAL uTREADY_TXD_0 : UNSIGNED(0 DOWNTO 0);  -- is slave ready for new data
    SIGNAL uTVALID_TXD_0 : UNSIGNED(0 DOWNTO 0);  -- master signaling valid data
    SIGNAL uTLAST_TXD_0  : UNSIGNED(0 DOWNTO 0);  -- last data of packet

    SIGNAL TDATA_TXD_1  : STD_LOGIC_VECTOR(31 DOWNTO 0); -- data, which is transmitted
    SIGNAL TREADY_TXD_1 : STD_LOGIC;                     -- is slave ready for new data
    SIGNAL TVALID_TXD_1 : STD_LOGIC;                     -- master signaling valid data
    SIGNAL TLAST_TXD_1  : STD_LOGIC;                     -- last data of packet

    SIGNAL uTDATA_TXD_1  : UNSIGNED(31 DOWNTO 0); -- data, which is transmitted
    SIGNAL uTREADY_TXD_1 : UNSIGNED(0 DOWNTO 0);  -- is slave ready for new data
    SIGNAL uTVALID_TXD_1 : UNSIGNED(0 DOWNTO 0);  -- master signaling valid data
    SIGNAL uTLAST_TXD_1  : UNSIGNED(0 DOWNTO 0);  -- last data of packet

BEGIN

    pipelinec_top_0 : ENTITY work.pipelinec_top
        PORT MAP(
            -- All clocks
            clk_100p0 => clk_100p0,

            -- slave axi receive data from other module
            s_axis0_tdata  => uTDATA_TXD_0,
            s_axis0_tkeep  => "0000",
            s_axis0_tlast  => uTLAST_TXD_0,
            s_axis0_tvalid => uTVALID_TXD_0,
            s_axis0_tready => uTREADY_TXD_0,

            s_axis1_tdata  => uTDATA_TXD_1,
            s_axis1_tkeep  => "0000",
            s_axis1_tlast  => uTLAST_TXD_1,
            s_axis1_tvalid => uTVALID_TXD_1,
            s_axis1_tready => uTREADY_TXD_1,

            -- master axi send data to other module
            m_axis0_tdata  => uTDATA_RXD_0,
            m_axis0_tkeep  => OPEN,
            m_axis0_tlast  => uTLAST_RXD_0,
            m_axis0_tvalid => uTVALID_RXD_0,
            m_axis0_tready => uTREADY_RXD_0
        );

    axi_slave_0 : ENTITY work.axi_slave
        GENERIC MAP(
            FIFO_SIZE => 1
        )
        PORT MAP(
            ACLK    => clk_100p0,
            ARESETn => '1',

            TDATA_RXD  => TDATA_RXD_0,
            TREADY_RXD => TREADY_RXD_0,
            TVALID_RXD => TVALID_RXD_0,
            TLAST_RXD  => TLAST_RXD_0,

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

            TDATA_TXD  => TDATA_TXD_0,
            TREADY_TXD => TREADY_TXD_0,
            TVALID_TXD => TVALID_TXD_0,
            TLAST_TXD  => TLAST_TXD_0
        );
    axi_master_1 : ENTITY work.axi_master
        GENERIC MAP(
            LENGTH => 100
        )
        PORT MAP(
            ACLK    => clk_100p0,
            ARESETn => '1',

            Trigger => trigger,

            TDATA_TXD  => TDATA_TXD_1,
            TREADY_TXD => TREADY_TXD_1,
            TVALID_TXD => TVALID_TXD_1,
            TLAST_TXD  => TLAST_TXD_1
        );

    ----------------------------------------------------------------------------
    uTDATA_TXD_0     <= UNSIGNED(TDATA_TXD_0);
    TREADY_TXD_0     <= STD_LOGIC(uTREADY_TXD_0(0));
    uTVALID_TXD_0(0) <= TVALID_TXD_0;

    uTDATA_TXD_1     <= UNSIGNED(TDATA_TXD_1);
    TREADY_TXD_1     <= STD_LOGIC(uTREADY_TXD_1(0));
    uTVALID_TXD_1(0) <= TVALID_TXD_1;

    TDATA_RXD_0      <= STD_LOGIC_VECTOR(uTDATA_RXD_0);
    uTREADY_RXD_0(0) <= TREADY_RXD_0;
    TVALID_RXD_0     <= STD_LOGIC(uTVALID_RXD_0(0));
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
