-- (c) Copyright 1995-2017 Xilinx, Inc. All rights reserved.
-- 
-- This file contains confidential and proprietary information
-- of Xilinx, Inc. and is protected under U.S. and
-- international copyright and other intellectual property
-- laws.
-- 
-- DISCLAIMER
-- This disclaimer is not a license and does not grant any
-- rights to the materials distributed herewith. Except as
-- otherwise provided in a valid license issued to you by
-- Xilinx, and to the maximum extent permitted by applicable
-- law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND
-- WITH ALL FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES
-- AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, INCLUDING
-- BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-
-- INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE; and
-- (2) Xilinx shall not be liable (whether in contract or tort,
-- including negligence, or under any other theory of
-- liability) for any loss or damage of any kind or nature
-- related to, arising under or in connection with these
-- materials, including for any direct, or any indirect,
-- special, incidental, or consequential loss or damage
-- (including loss of data, profits, goodwill, or any type of
-- loss or damage suffered as a result of any action brought
-- by a third party) even if such damage or loss was
-- reasonably foreseeable or Xilinx had been advised of the
-- possibility of the same.
-- 
-- CRITICAL APPLICATIONS
-- Xilinx products are not designed or intended to be fail-
-- safe, or for use in any application requiring fail-safe
-- performance, such as life-support or safety devices or
-- systems, Class III medical devices, nuclear facilities,
-- applications related to the deployment of airbags, or any
-- other applications that could lead to death, personal
-- injury, or severe property or environmental damage
-- (individually and collectively, "Critical
-- Applications"). Customer assumes the sole risk and
-- liability of any use of Xilinx products in Critical
-- Applications, subject only to applicable laws and
-- regulations governing limitations on product liability.
-- 
-- THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS
-- PART OF THIS FILE AT ALL TIMES.
-- 
-- DO NOT MODIFY THIS FILE.

-- IP VLNV: xilinx.com:hls:convHW:1.0
-- IP Revision: 1711221039

LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.numeric_std.ALL;

ENTITY system_convHW_0_0 IS
  PORT (
    s_axi_AXILiteS_AWADDR : IN STD_LOGIC_VECTOR(4 DOWNTO 0);
    s_axi_AXILiteS_AWVALID : IN STD_LOGIC;
    s_axi_AXILiteS_AWREADY : OUT STD_LOGIC;
    s_axi_AXILiteS_WDATA : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    s_axi_AXILiteS_WSTRB : IN STD_LOGIC_VECTOR(3 DOWNTO 0);
    s_axi_AXILiteS_WVALID : IN STD_LOGIC;
    s_axi_AXILiteS_WREADY : OUT STD_LOGIC;
    s_axi_AXILiteS_BRESP : OUT STD_LOGIC_VECTOR(1 DOWNTO 0);
    s_axi_AXILiteS_BVALID : OUT STD_LOGIC;
    s_axi_AXILiteS_BREADY : IN STD_LOGIC;
    s_axi_AXILiteS_ARADDR : IN STD_LOGIC_VECTOR(4 DOWNTO 0);
    s_axi_AXILiteS_ARVALID : IN STD_LOGIC;
    s_axi_AXILiteS_ARREADY : OUT STD_LOGIC;
    s_axi_AXILiteS_RDATA : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
    s_axi_AXILiteS_RRESP : OUT STD_LOGIC_VECTOR(1 DOWNTO 0);
    s_axi_AXILiteS_RVALID : OUT STD_LOGIC;
    s_axi_AXILiteS_RREADY : IN STD_LOGIC;
    ap_clk : IN STD_LOGIC;
    ap_rst_n : IN STD_LOGIC;
    interrupt : OUT STD_LOGIC;
    m_axi_WeightsIn_AWADDR : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
    m_axi_WeightsIn_AWLEN : OUT STD_LOGIC_VECTOR(7 DOWNTO 0);
    m_axi_WeightsIn_AWSIZE : OUT STD_LOGIC_VECTOR(2 DOWNTO 0);
    m_axi_WeightsIn_AWBURST : OUT STD_LOGIC_VECTOR(1 DOWNTO 0);
    m_axi_WeightsIn_AWLOCK : OUT STD_LOGIC_VECTOR(1 DOWNTO 0);
    m_axi_WeightsIn_AWREGION : OUT STD_LOGIC_VECTOR(3 DOWNTO 0);
    m_axi_WeightsIn_AWCACHE : OUT STD_LOGIC_VECTOR(3 DOWNTO 0);
    m_axi_WeightsIn_AWPROT : OUT STD_LOGIC_VECTOR(2 DOWNTO 0);
    m_axi_WeightsIn_AWQOS : OUT STD_LOGIC_VECTOR(3 DOWNTO 0);
    m_axi_WeightsIn_AWVALID : OUT STD_LOGIC;
    m_axi_WeightsIn_AWREADY : IN STD_LOGIC;
    m_axi_WeightsIn_WDATA : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
    m_axi_WeightsIn_WSTRB : OUT STD_LOGIC_VECTOR(3 DOWNTO 0);
    m_axi_WeightsIn_WLAST : OUT STD_LOGIC;
    m_axi_WeightsIn_WVALID : OUT STD_LOGIC;
    m_axi_WeightsIn_WREADY : IN STD_LOGIC;
    m_axi_WeightsIn_BRESP : IN STD_LOGIC_VECTOR(1 DOWNTO 0);
    m_axi_WeightsIn_BVALID : IN STD_LOGIC;
    m_axi_WeightsIn_BREADY : OUT STD_LOGIC;
    m_axi_WeightsIn_ARADDR : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
    m_axi_WeightsIn_ARLEN : OUT STD_LOGIC_VECTOR(7 DOWNTO 0);
    m_axi_WeightsIn_ARSIZE : OUT STD_LOGIC_VECTOR(2 DOWNTO 0);
    m_axi_WeightsIn_ARBURST : OUT STD_LOGIC_VECTOR(1 DOWNTO 0);
    m_axi_WeightsIn_ARLOCK : OUT STD_LOGIC_VECTOR(1 DOWNTO 0);
    m_axi_WeightsIn_ARREGION : OUT STD_LOGIC_VECTOR(3 DOWNTO 0);
    m_axi_WeightsIn_ARCACHE : OUT STD_LOGIC_VECTOR(3 DOWNTO 0);
    m_axi_WeightsIn_ARPROT : OUT STD_LOGIC_VECTOR(2 DOWNTO 0);
    m_axi_WeightsIn_ARQOS : OUT STD_LOGIC_VECTOR(3 DOWNTO 0);
    m_axi_WeightsIn_ARVALID : OUT STD_LOGIC;
    m_axi_WeightsIn_ARREADY : IN STD_LOGIC;
    m_axi_WeightsIn_RDATA : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    m_axi_WeightsIn_RRESP : IN STD_LOGIC_VECTOR(1 DOWNTO 0);
    m_axi_WeightsIn_RLAST : IN STD_LOGIC;
    m_axi_WeightsIn_RVALID : IN STD_LOGIC;
    m_axi_WeightsIn_RREADY : OUT STD_LOGIC;
    input_V_TVALID : IN STD_LOGIC;
    input_V_TREADY : OUT STD_LOGIC;
    input_V_TDATA : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    outputStream_V_TVALID : OUT STD_LOGIC;
    outputStream_V_TREADY : IN STD_LOGIC;
    outputStream_V_TDATA : OUT STD_LOGIC_VECTOR(31 DOWNTO 0)
  );
END system_convHW_0_0;

ARCHITECTURE system_convHW_0_0_arch OF system_convHW_0_0 IS
  ATTRIBUTE DowngradeIPIdentifiedWarnings : STRING;
  ATTRIBUTE DowngradeIPIdentifiedWarnings OF system_convHW_0_0_arch: ARCHITECTURE IS "yes";
  COMPONENT convHW IS
    GENERIC (
      C_S_AXI_AXILITES_ADDR_WIDTH : INTEGER;
      C_S_AXI_AXILITES_DATA_WIDTH : INTEGER;
      C_M_AXI_WEIGHTSIN_ID_WIDTH : INTEGER;
      C_M_AXI_WEIGHTSIN_ADDR_WIDTH : INTEGER;
      C_M_AXI_WEIGHTSIN_DATA_WIDTH : INTEGER;
      C_M_AXI_WEIGHTSIN_AWUSER_WIDTH : INTEGER;
      C_M_AXI_WEIGHTSIN_ARUSER_WIDTH : INTEGER;
      C_M_AXI_WEIGHTSIN_WUSER_WIDTH : INTEGER;
      C_M_AXI_WEIGHTSIN_RUSER_WIDTH : INTEGER;
      C_M_AXI_WEIGHTSIN_BUSER_WIDTH : INTEGER;
      C_M_AXI_WEIGHTSIN_USER_VALUE : INTEGER;
      C_M_AXI_WEIGHTSIN_PROT_VALUE : INTEGER;
      C_M_AXI_WEIGHTSIN_CACHE_VALUE : INTEGER
    );
    PORT (
      s_axi_AXILiteS_AWADDR : IN STD_LOGIC_VECTOR(4 DOWNTO 0);
      s_axi_AXILiteS_AWVALID : IN STD_LOGIC;
      s_axi_AXILiteS_AWREADY : OUT STD_LOGIC;
      s_axi_AXILiteS_WDATA : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
      s_axi_AXILiteS_WSTRB : IN STD_LOGIC_VECTOR(3 DOWNTO 0);
      s_axi_AXILiteS_WVALID : IN STD_LOGIC;
      s_axi_AXILiteS_WREADY : OUT STD_LOGIC;
      s_axi_AXILiteS_BRESP : OUT STD_LOGIC_VECTOR(1 DOWNTO 0);
      s_axi_AXILiteS_BVALID : OUT STD_LOGIC;
      s_axi_AXILiteS_BREADY : IN STD_LOGIC;
      s_axi_AXILiteS_ARADDR : IN STD_LOGIC_VECTOR(4 DOWNTO 0);
      s_axi_AXILiteS_ARVALID : IN STD_LOGIC;
      s_axi_AXILiteS_ARREADY : OUT STD_LOGIC;
      s_axi_AXILiteS_RDATA : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
      s_axi_AXILiteS_RRESP : OUT STD_LOGIC_VECTOR(1 DOWNTO 0);
      s_axi_AXILiteS_RVALID : OUT STD_LOGIC;
      s_axi_AXILiteS_RREADY : IN STD_LOGIC;
      ap_clk : IN STD_LOGIC;
      ap_rst_n : IN STD_LOGIC;
      interrupt : OUT STD_LOGIC;
      m_axi_WeightsIn_AWID : OUT STD_LOGIC_VECTOR(0 DOWNTO 0);
      m_axi_WeightsIn_AWADDR : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
      m_axi_WeightsIn_AWLEN : OUT STD_LOGIC_VECTOR(7 DOWNTO 0);
      m_axi_WeightsIn_AWSIZE : OUT STD_LOGIC_VECTOR(2 DOWNTO 0);
      m_axi_WeightsIn_AWBURST : OUT STD_LOGIC_VECTOR(1 DOWNTO 0);
      m_axi_WeightsIn_AWLOCK : OUT STD_LOGIC_VECTOR(1 DOWNTO 0);
      m_axi_WeightsIn_AWREGION : OUT STD_LOGIC_VECTOR(3 DOWNTO 0);
      m_axi_WeightsIn_AWCACHE : OUT STD_LOGIC_VECTOR(3 DOWNTO 0);
      m_axi_WeightsIn_AWPROT : OUT STD_LOGIC_VECTOR(2 DOWNTO 0);
      m_axi_WeightsIn_AWQOS : OUT STD_LOGIC_VECTOR(3 DOWNTO 0);
      m_axi_WeightsIn_AWUSER : OUT STD_LOGIC_VECTOR(0 DOWNTO 0);
      m_axi_WeightsIn_AWVALID : OUT STD_LOGIC;
      m_axi_WeightsIn_AWREADY : IN STD_LOGIC;
      m_axi_WeightsIn_WID : OUT STD_LOGIC_VECTOR(0 DOWNTO 0);
      m_axi_WeightsIn_WDATA : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
      m_axi_WeightsIn_WSTRB : OUT STD_LOGIC_VECTOR(3 DOWNTO 0);
      m_axi_WeightsIn_WLAST : OUT STD_LOGIC;
      m_axi_WeightsIn_WUSER : OUT STD_LOGIC_VECTOR(0 DOWNTO 0);
      m_axi_WeightsIn_WVALID : OUT STD_LOGIC;
      m_axi_WeightsIn_WREADY : IN STD_LOGIC;
      m_axi_WeightsIn_BID : IN STD_LOGIC_VECTOR(0 DOWNTO 0);
      m_axi_WeightsIn_BRESP : IN STD_LOGIC_VECTOR(1 DOWNTO 0);
      m_axi_WeightsIn_BUSER : IN STD_LOGIC_VECTOR(0 DOWNTO 0);
      m_axi_WeightsIn_BVALID : IN STD_LOGIC;
      m_axi_WeightsIn_BREADY : OUT STD_LOGIC;
      m_axi_WeightsIn_ARID : OUT STD_LOGIC_VECTOR(0 DOWNTO 0);
      m_axi_WeightsIn_ARADDR : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
      m_axi_WeightsIn_ARLEN : OUT STD_LOGIC_VECTOR(7 DOWNTO 0);
      m_axi_WeightsIn_ARSIZE : OUT STD_LOGIC_VECTOR(2 DOWNTO 0);
      m_axi_WeightsIn_ARBURST : OUT STD_LOGIC_VECTOR(1 DOWNTO 0);
      m_axi_WeightsIn_ARLOCK : OUT STD_LOGIC_VECTOR(1 DOWNTO 0);
      m_axi_WeightsIn_ARREGION : OUT STD_LOGIC_VECTOR(3 DOWNTO 0);
      m_axi_WeightsIn_ARCACHE : OUT STD_LOGIC_VECTOR(3 DOWNTO 0);
      m_axi_WeightsIn_ARPROT : OUT STD_LOGIC_VECTOR(2 DOWNTO 0);
      m_axi_WeightsIn_ARQOS : OUT STD_LOGIC_VECTOR(3 DOWNTO 0);
      m_axi_WeightsIn_ARUSER : OUT STD_LOGIC_VECTOR(0 DOWNTO 0);
      m_axi_WeightsIn_ARVALID : OUT STD_LOGIC;
      m_axi_WeightsIn_ARREADY : IN STD_LOGIC;
      m_axi_WeightsIn_RID : IN STD_LOGIC_VECTOR(0 DOWNTO 0);
      m_axi_WeightsIn_RDATA : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
      m_axi_WeightsIn_RRESP : IN STD_LOGIC_VECTOR(1 DOWNTO 0);
      m_axi_WeightsIn_RLAST : IN STD_LOGIC;
      m_axi_WeightsIn_RUSER : IN STD_LOGIC_VECTOR(0 DOWNTO 0);
      m_axi_WeightsIn_RVALID : IN STD_LOGIC;
      m_axi_WeightsIn_RREADY : OUT STD_LOGIC;
      input_V_TVALID : IN STD_LOGIC;
      input_V_TREADY : OUT STD_LOGIC;
      input_V_TDATA : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
      outputStream_V_TVALID : OUT STD_LOGIC;
      outputStream_V_TREADY : IN STD_LOGIC;
      outputStream_V_TDATA : OUT STD_LOGIC_VECTOR(31 DOWNTO 0)
    );
  END COMPONENT convHW;
  ATTRIBUTE X_INTERFACE_INFO : STRING;
  ATTRIBUTE X_INTERFACE_INFO OF s_axi_AXILiteS_AWADDR: SIGNAL IS "xilinx.com:interface:aximm:1.0 s_axi_AXILiteS AWADDR";
  ATTRIBUTE X_INTERFACE_INFO OF s_axi_AXILiteS_AWVALID: SIGNAL IS "xilinx.com:interface:aximm:1.0 s_axi_AXILiteS AWVALID";
  ATTRIBUTE X_INTERFACE_INFO OF s_axi_AXILiteS_AWREADY: SIGNAL IS "xilinx.com:interface:aximm:1.0 s_axi_AXILiteS AWREADY";
  ATTRIBUTE X_INTERFACE_INFO OF s_axi_AXILiteS_WDATA: SIGNAL IS "xilinx.com:interface:aximm:1.0 s_axi_AXILiteS WDATA";
  ATTRIBUTE X_INTERFACE_INFO OF s_axi_AXILiteS_WSTRB: SIGNAL IS "xilinx.com:interface:aximm:1.0 s_axi_AXILiteS WSTRB";
  ATTRIBUTE X_INTERFACE_INFO OF s_axi_AXILiteS_WVALID: SIGNAL IS "xilinx.com:interface:aximm:1.0 s_axi_AXILiteS WVALID";
  ATTRIBUTE X_INTERFACE_INFO OF s_axi_AXILiteS_WREADY: SIGNAL IS "xilinx.com:interface:aximm:1.0 s_axi_AXILiteS WREADY";
  ATTRIBUTE X_INTERFACE_INFO OF s_axi_AXILiteS_BRESP: SIGNAL IS "xilinx.com:interface:aximm:1.0 s_axi_AXILiteS BRESP";
  ATTRIBUTE X_INTERFACE_INFO OF s_axi_AXILiteS_BVALID: SIGNAL IS "xilinx.com:interface:aximm:1.0 s_axi_AXILiteS BVALID";
  ATTRIBUTE X_INTERFACE_INFO OF s_axi_AXILiteS_BREADY: SIGNAL IS "xilinx.com:interface:aximm:1.0 s_axi_AXILiteS BREADY";
  ATTRIBUTE X_INTERFACE_INFO OF s_axi_AXILiteS_ARADDR: SIGNAL IS "xilinx.com:interface:aximm:1.0 s_axi_AXILiteS ARADDR";
  ATTRIBUTE X_INTERFACE_INFO OF s_axi_AXILiteS_ARVALID: SIGNAL IS "xilinx.com:interface:aximm:1.0 s_axi_AXILiteS ARVALID";
  ATTRIBUTE X_INTERFACE_INFO OF s_axi_AXILiteS_ARREADY: SIGNAL IS "xilinx.com:interface:aximm:1.0 s_axi_AXILiteS ARREADY";
  ATTRIBUTE X_INTERFACE_INFO OF s_axi_AXILiteS_RDATA: SIGNAL IS "xilinx.com:interface:aximm:1.0 s_axi_AXILiteS RDATA";
  ATTRIBUTE X_INTERFACE_INFO OF s_axi_AXILiteS_RRESP: SIGNAL IS "xilinx.com:interface:aximm:1.0 s_axi_AXILiteS RRESP";
  ATTRIBUTE X_INTERFACE_INFO OF s_axi_AXILiteS_RVALID: SIGNAL IS "xilinx.com:interface:aximm:1.0 s_axi_AXILiteS RVALID";
  ATTRIBUTE X_INTERFACE_INFO OF s_axi_AXILiteS_RREADY: SIGNAL IS "xilinx.com:interface:aximm:1.0 s_axi_AXILiteS RREADY";
  ATTRIBUTE X_INTERFACE_INFO OF ap_clk: SIGNAL IS "xilinx.com:signal:clock:1.0 ap_clk CLK";
  ATTRIBUTE X_INTERFACE_INFO OF ap_rst_n: SIGNAL IS "xilinx.com:signal:reset:1.0 ap_rst_n RST";
  ATTRIBUTE X_INTERFACE_INFO OF interrupt: SIGNAL IS "xilinx.com:signal:interrupt:1.0 interrupt INTERRUPT";
  ATTRIBUTE X_INTERFACE_INFO OF m_axi_WeightsIn_AWADDR: SIGNAL IS "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn AWADDR";
  ATTRIBUTE X_INTERFACE_INFO OF m_axi_WeightsIn_AWLEN: SIGNAL IS "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn AWLEN";
  ATTRIBUTE X_INTERFACE_INFO OF m_axi_WeightsIn_AWSIZE: SIGNAL IS "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn AWSIZE";
  ATTRIBUTE X_INTERFACE_INFO OF m_axi_WeightsIn_AWBURST: SIGNAL IS "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn AWBURST";
  ATTRIBUTE X_INTERFACE_INFO OF m_axi_WeightsIn_AWLOCK: SIGNAL IS "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn AWLOCK";
  ATTRIBUTE X_INTERFACE_INFO OF m_axi_WeightsIn_AWREGION: SIGNAL IS "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn AWREGION";
  ATTRIBUTE X_INTERFACE_INFO OF m_axi_WeightsIn_AWCACHE: SIGNAL IS "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn AWCACHE";
  ATTRIBUTE X_INTERFACE_INFO OF m_axi_WeightsIn_AWPROT: SIGNAL IS "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn AWPROT";
  ATTRIBUTE X_INTERFACE_INFO OF m_axi_WeightsIn_AWQOS: SIGNAL IS "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn AWQOS";
  ATTRIBUTE X_INTERFACE_INFO OF m_axi_WeightsIn_AWVALID: SIGNAL IS "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn AWVALID";
  ATTRIBUTE X_INTERFACE_INFO OF m_axi_WeightsIn_AWREADY: SIGNAL IS "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn AWREADY";
  ATTRIBUTE X_INTERFACE_INFO OF m_axi_WeightsIn_WDATA: SIGNAL IS "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn WDATA";
  ATTRIBUTE X_INTERFACE_INFO OF m_axi_WeightsIn_WSTRB: SIGNAL IS "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn WSTRB";
  ATTRIBUTE X_INTERFACE_INFO OF m_axi_WeightsIn_WLAST: SIGNAL IS "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn WLAST";
  ATTRIBUTE X_INTERFACE_INFO OF m_axi_WeightsIn_WVALID: SIGNAL IS "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn WVALID";
  ATTRIBUTE X_INTERFACE_INFO OF m_axi_WeightsIn_WREADY: SIGNAL IS "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn WREADY";
  ATTRIBUTE X_INTERFACE_INFO OF m_axi_WeightsIn_BRESP: SIGNAL IS "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn BRESP";
  ATTRIBUTE X_INTERFACE_INFO OF m_axi_WeightsIn_BVALID: SIGNAL IS "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn BVALID";
  ATTRIBUTE X_INTERFACE_INFO OF m_axi_WeightsIn_BREADY: SIGNAL IS "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn BREADY";
  ATTRIBUTE X_INTERFACE_INFO OF m_axi_WeightsIn_ARADDR: SIGNAL IS "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn ARADDR";
  ATTRIBUTE X_INTERFACE_INFO OF m_axi_WeightsIn_ARLEN: SIGNAL IS "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn ARLEN";
  ATTRIBUTE X_INTERFACE_INFO OF m_axi_WeightsIn_ARSIZE: SIGNAL IS "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn ARSIZE";
  ATTRIBUTE X_INTERFACE_INFO OF m_axi_WeightsIn_ARBURST: SIGNAL IS "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn ARBURST";
  ATTRIBUTE X_INTERFACE_INFO OF m_axi_WeightsIn_ARLOCK: SIGNAL IS "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn ARLOCK";
  ATTRIBUTE X_INTERFACE_INFO OF m_axi_WeightsIn_ARREGION: SIGNAL IS "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn ARREGION";
  ATTRIBUTE X_INTERFACE_INFO OF m_axi_WeightsIn_ARCACHE: SIGNAL IS "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn ARCACHE";
  ATTRIBUTE X_INTERFACE_INFO OF m_axi_WeightsIn_ARPROT: SIGNAL IS "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn ARPROT";
  ATTRIBUTE X_INTERFACE_INFO OF m_axi_WeightsIn_ARQOS: SIGNAL IS "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn ARQOS";
  ATTRIBUTE X_INTERFACE_INFO OF m_axi_WeightsIn_ARVALID: SIGNAL IS "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn ARVALID";
  ATTRIBUTE X_INTERFACE_INFO OF m_axi_WeightsIn_ARREADY: SIGNAL IS "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn ARREADY";
  ATTRIBUTE X_INTERFACE_INFO OF m_axi_WeightsIn_RDATA: SIGNAL IS "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn RDATA";
  ATTRIBUTE X_INTERFACE_INFO OF m_axi_WeightsIn_RRESP: SIGNAL IS "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn RRESP";
  ATTRIBUTE X_INTERFACE_INFO OF m_axi_WeightsIn_RLAST: SIGNAL IS "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn RLAST";
  ATTRIBUTE X_INTERFACE_INFO OF m_axi_WeightsIn_RVALID: SIGNAL IS "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn RVALID";
  ATTRIBUTE X_INTERFACE_INFO OF m_axi_WeightsIn_RREADY: SIGNAL IS "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn RREADY";
  ATTRIBUTE X_INTERFACE_INFO OF input_V_TVALID: SIGNAL IS "xilinx.com:interface:axis:1.0 input_V TVALID";
  ATTRIBUTE X_INTERFACE_INFO OF input_V_TREADY: SIGNAL IS "xilinx.com:interface:axis:1.0 input_V TREADY";
  ATTRIBUTE X_INTERFACE_INFO OF input_V_TDATA: SIGNAL IS "xilinx.com:interface:axis:1.0 input_V TDATA";
  ATTRIBUTE X_INTERFACE_INFO OF outputStream_V_TVALID: SIGNAL IS "xilinx.com:interface:axis:1.0 outputStream_V TVALID";
  ATTRIBUTE X_INTERFACE_INFO OF outputStream_V_TREADY: SIGNAL IS "xilinx.com:interface:axis:1.0 outputStream_V TREADY";
  ATTRIBUTE X_INTERFACE_INFO OF outputStream_V_TDATA: SIGNAL IS "xilinx.com:interface:axis:1.0 outputStream_V TDATA";
BEGIN
  U0 : convHW
    GENERIC MAP (
      C_S_AXI_AXILITES_ADDR_WIDTH => 5,
      C_S_AXI_AXILITES_DATA_WIDTH => 32,
      C_M_AXI_WEIGHTSIN_ID_WIDTH => 1,
      C_M_AXI_WEIGHTSIN_ADDR_WIDTH => 32,
      C_M_AXI_WEIGHTSIN_DATA_WIDTH => 32,
      C_M_AXI_WEIGHTSIN_AWUSER_WIDTH => 1,
      C_M_AXI_WEIGHTSIN_ARUSER_WIDTH => 1,
      C_M_AXI_WEIGHTSIN_WUSER_WIDTH => 1,
      C_M_AXI_WEIGHTSIN_RUSER_WIDTH => 1,
      C_M_AXI_WEIGHTSIN_BUSER_WIDTH => 1,
      C_M_AXI_WEIGHTSIN_USER_VALUE => 0,
      C_M_AXI_WEIGHTSIN_PROT_VALUE => 0,
      C_M_AXI_WEIGHTSIN_CACHE_VALUE => 3
    )
    PORT MAP (
      s_axi_AXILiteS_AWADDR => s_axi_AXILiteS_AWADDR,
      s_axi_AXILiteS_AWVALID => s_axi_AXILiteS_AWVALID,
      s_axi_AXILiteS_AWREADY => s_axi_AXILiteS_AWREADY,
      s_axi_AXILiteS_WDATA => s_axi_AXILiteS_WDATA,
      s_axi_AXILiteS_WSTRB => s_axi_AXILiteS_WSTRB,
      s_axi_AXILiteS_WVALID => s_axi_AXILiteS_WVALID,
      s_axi_AXILiteS_WREADY => s_axi_AXILiteS_WREADY,
      s_axi_AXILiteS_BRESP => s_axi_AXILiteS_BRESP,
      s_axi_AXILiteS_BVALID => s_axi_AXILiteS_BVALID,
      s_axi_AXILiteS_BREADY => s_axi_AXILiteS_BREADY,
      s_axi_AXILiteS_ARADDR => s_axi_AXILiteS_ARADDR,
      s_axi_AXILiteS_ARVALID => s_axi_AXILiteS_ARVALID,
      s_axi_AXILiteS_ARREADY => s_axi_AXILiteS_ARREADY,
      s_axi_AXILiteS_RDATA => s_axi_AXILiteS_RDATA,
      s_axi_AXILiteS_RRESP => s_axi_AXILiteS_RRESP,
      s_axi_AXILiteS_RVALID => s_axi_AXILiteS_RVALID,
      s_axi_AXILiteS_RREADY => s_axi_AXILiteS_RREADY,
      ap_clk => ap_clk,
      ap_rst_n => ap_rst_n,
      interrupt => interrupt,
      m_axi_WeightsIn_AWADDR => m_axi_WeightsIn_AWADDR,
      m_axi_WeightsIn_AWLEN => m_axi_WeightsIn_AWLEN,
      m_axi_WeightsIn_AWSIZE => m_axi_WeightsIn_AWSIZE,
      m_axi_WeightsIn_AWBURST => m_axi_WeightsIn_AWBURST,
      m_axi_WeightsIn_AWLOCK => m_axi_WeightsIn_AWLOCK,
      m_axi_WeightsIn_AWREGION => m_axi_WeightsIn_AWREGION,
      m_axi_WeightsIn_AWCACHE => m_axi_WeightsIn_AWCACHE,
      m_axi_WeightsIn_AWPROT => m_axi_WeightsIn_AWPROT,
      m_axi_WeightsIn_AWQOS => m_axi_WeightsIn_AWQOS,
      m_axi_WeightsIn_AWVALID => m_axi_WeightsIn_AWVALID,
      m_axi_WeightsIn_AWREADY => m_axi_WeightsIn_AWREADY,
      m_axi_WeightsIn_WDATA => m_axi_WeightsIn_WDATA,
      m_axi_WeightsIn_WSTRB => m_axi_WeightsIn_WSTRB,
      m_axi_WeightsIn_WLAST => m_axi_WeightsIn_WLAST,
      m_axi_WeightsIn_WVALID => m_axi_WeightsIn_WVALID,
      m_axi_WeightsIn_WREADY => m_axi_WeightsIn_WREADY,
      m_axi_WeightsIn_BID => STD_LOGIC_VECTOR(TO_UNSIGNED(0, 1)),
      m_axi_WeightsIn_BRESP => m_axi_WeightsIn_BRESP,
      m_axi_WeightsIn_BUSER => STD_LOGIC_VECTOR(TO_UNSIGNED(0, 1)),
      m_axi_WeightsIn_BVALID => m_axi_WeightsIn_BVALID,
      m_axi_WeightsIn_BREADY => m_axi_WeightsIn_BREADY,
      m_axi_WeightsIn_ARADDR => m_axi_WeightsIn_ARADDR,
      m_axi_WeightsIn_ARLEN => m_axi_WeightsIn_ARLEN,
      m_axi_WeightsIn_ARSIZE => m_axi_WeightsIn_ARSIZE,
      m_axi_WeightsIn_ARBURST => m_axi_WeightsIn_ARBURST,
      m_axi_WeightsIn_ARLOCK => m_axi_WeightsIn_ARLOCK,
      m_axi_WeightsIn_ARREGION => m_axi_WeightsIn_ARREGION,
      m_axi_WeightsIn_ARCACHE => m_axi_WeightsIn_ARCACHE,
      m_axi_WeightsIn_ARPROT => m_axi_WeightsIn_ARPROT,
      m_axi_WeightsIn_ARQOS => m_axi_WeightsIn_ARQOS,
      m_axi_WeightsIn_ARVALID => m_axi_WeightsIn_ARVALID,
      m_axi_WeightsIn_ARREADY => m_axi_WeightsIn_ARREADY,
      m_axi_WeightsIn_RID => STD_LOGIC_VECTOR(TO_UNSIGNED(0, 1)),
      m_axi_WeightsIn_RDATA => m_axi_WeightsIn_RDATA,
      m_axi_WeightsIn_RRESP => m_axi_WeightsIn_RRESP,
      m_axi_WeightsIn_RLAST => m_axi_WeightsIn_RLAST,
      m_axi_WeightsIn_RUSER => STD_LOGIC_VECTOR(TO_UNSIGNED(0, 1)),
      m_axi_WeightsIn_RVALID => m_axi_WeightsIn_RVALID,
      m_axi_WeightsIn_RREADY => m_axi_WeightsIn_RREADY,
      input_V_TVALID => input_V_TVALID,
      input_V_TREADY => input_V_TREADY,
      input_V_TDATA => input_V_TDATA,
      outputStream_V_TVALID => outputStream_V_TVALID,
      outputStream_V_TREADY => outputStream_V_TREADY,
      outputStream_V_TDATA => outputStream_V_TDATA
    );
END system_convHW_0_0_arch;
