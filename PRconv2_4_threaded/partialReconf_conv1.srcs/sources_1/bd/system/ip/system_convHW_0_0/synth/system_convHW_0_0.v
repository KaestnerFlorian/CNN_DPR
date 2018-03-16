// (c) Copyright 1995-2017 Xilinx, Inc. All rights reserved.
// 
// This file contains confidential and proprietary information
// of Xilinx, Inc. and is protected under U.S. and
// international copyright and other intellectual property
// laws.
// 
// DISCLAIMER
// This disclaimer is not a license and does not grant any
// rights to the materials distributed herewith. Except as
// otherwise provided in a valid license issued to you by
// Xilinx, and to the maximum extent permitted by applicable
// law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND
// WITH ALL FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES
// AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, INCLUDING
// BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-
// INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE; and
// (2) Xilinx shall not be liable (whether in contract or tort,
// including negligence, or under any other theory of
// liability) for any loss or damage of any kind or nature
// related to, arising under or in connection with these
// materials, including for any direct, or any indirect,
// special, incidental, or consequential loss or damage
// (including loss of data, profits, goodwill, or any type of
// loss or damage suffered as a result of any action brought
// by a third party) even if such damage or loss was
// reasonably foreseeable or Xilinx had been advised of the
// possibility of the same.
// 
// CRITICAL APPLICATIONS
// Xilinx products are not designed or intended to be fail-
// safe, or for use in any application requiring fail-safe
// performance, such as life-support or safety devices or
// systems, Class III medical devices, nuclear facilities,
// applications related to the deployment of airbags, or any
// other applications that could lead to death, personal
// injury, or severe property or environmental damage
// (individually and collectively, "Critical
// Applications"). Customer assumes the sole risk and
// liability of any use of Xilinx products in Critical
// Applications, subject only to applicable laws and
// regulations governing limitations on product liability.
// 
// THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS
// PART OF THIS FILE AT ALL TIMES.
// 
// DO NOT MODIFY THIS FILE.


// IP VLNV: xilinx.com:hls:convHW:1.0
// IP Revision: 1711221039

(* X_CORE_INFO = "convHW,Vivado 2017.1" *)
(* CHECK_LICENSE_TYPE = "system_convHW_0_0,convHW,{}" *)
(* CORE_GENERATION_INFO = "system_convHW_0_0,convHW,{x_ipProduct=Vivado 2017.1,x_ipVendor=xilinx.com,x_ipLibrary=hls,x_ipName=convHW,x_ipVersion=1.0,x_ipCoreRevision=1711221039,x_ipLanguage=VERILOG,x_ipSimLanguage=MIXED,C_S_AXI_AXILITES_ADDR_WIDTH=5,C_S_AXI_AXILITES_DATA_WIDTH=32,C_M_AXI_WEIGHTSIN_ID_WIDTH=1,C_M_AXI_WEIGHTSIN_ADDR_WIDTH=32,C_M_AXI_WEIGHTSIN_DATA_WIDTH=32,C_M_AXI_WEIGHTSIN_AWUSER_WIDTH=1,C_M_AXI_WEIGHTSIN_ARUSER_WIDTH=1,C_M_AXI_WEIGHTSIN_WUSER_WIDTH=1,C_M_AXI_WEIGHTSIN_RUSER_WIDTH=1,C_M_AXI_WEIGHTSIN_BUSER\
_WIDTH=1,C_M_AXI_WEIGHTSIN_USER_VALUE=0x00000000,C_M_AXI_WEIGHTSIN_PROT_VALUE=000,C_M_AXI_WEIGHTSIN_CACHE_VALUE=0011}" *)
(* DowngradeIPIdentifiedWarnings = "yes" *)
module system_convHW_0_0 (
  s_axi_AXILiteS_AWADDR,
  s_axi_AXILiteS_AWVALID,
  s_axi_AXILiteS_AWREADY,
  s_axi_AXILiteS_WDATA,
  s_axi_AXILiteS_WSTRB,
  s_axi_AXILiteS_WVALID,
  s_axi_AXILiteS_WREADY,
  s_axi_AXILiteS_BRESP,
  s_axi_AXILiteS_BVALID,
  s_axi_AXILiteS_BREADY,
  s_axi_AXILiteS_ARADDR,
  s_axi_AXILiteS_ARVALID,
  s_axi_AXILiteS_ARREADY,
  s_axi_AXILiteS_RDATA,
  s_axi_AXILiteS_RRESP,
  s_axi_AXILiteS_RVALID,
  s_axi_AXILiteS_RREADY,
  ap_clk,
  ap_rst_n,
  interrupt,
  m_axi_WeightsIn_AWADDR,
  m_axi_WeightsIn_AWLEN,
  m_axi_WeightsIn_AWSIZE,
  m_axi_WeightsIn_AWBURST,
  m_axi_WeightsIn_AWLOCK,
  m_axi_WeightsIn_AWREGION,
  m_axi_WeightsIn_AWCACHE,
  m_axi_WeightsIn_AWPROT,
  m_axi_WeightsIn_AWQOS,
  m_axi_WeightsIn_AWVALID,
  m_axi_WeightsIn_AWREADY,
  m_axi_WeightsIn_WDATA,
  m_axi_WeightsIn_WSTRB,
  m_axi_WeightsIn_WLAST,
  m_axi_WeightsIn_WVALID,
  m_axi_WeightsIn_WREADY,
  m_axi_WeightsIn_BRESP,
  m_axi_WeightsIn_BVALID,
  m_axi_WeightsIn_BREADY,
  m_axi_WeightsIn_ARADDR,
  m_axi_WeightsIn_ARLEN,
  m_axi_WeightsIn_ARSIZE,
  m_axi_WeightsIn_ARBURST,
  m_axi_WeightsIn_ARLOCK,
  m_axi_WeightsIn_ARREGION,
  m_axi_WeightsIn_ARCACHE,
  m_axi_WeightsIn_ARPROT,
  m_axi_WeightsIn_ARQOS,
  m_axi_WeightsIn_ARVALID,
  m_axi_WeightsIn_ARREADY,
  m_axi_WeightsIn_RDATA,
  m_axi_WeightsIn_RRESP,
  m_axi_WeightsIn_RLAST,
  m_axi_WeightsIn_RVALID,
  m_axi_WeightsIn_RREADY,
  input_V_TVALID,
  input_V_TREADY,
  input_V_TDATA,
  outputStream_V_TVALID,
  outputStream_V_TREADY,
  outputStream_V_TDATA
);

(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 s_axi_AXILiteS AWADDR" *)
input wire [4 : 0] s_axi_AXILiteS_AWADDR;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 s_axi_AXILiteS AWVALID" *)
input wire s_axi_AXILiteS_AWVALID;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 s_axi_AXILiteS AWREADY" *)
output wire s_axi_AXILiteS_AWREADY;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 s_axi_AXILiteS WDATA" *)
input wire [31 : 0] s_axi_AXILiteS_WDATA;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 s_axi_AXILiteS WSTRB" *)
input wire [3 : 0] s_axi_AXILiteS_WSTRB;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 s_axi_AXILiteS WVALID" *)
input wire s_axi_AXILiteS_WVALID;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 s_axi_AXILiteS WREADY" *)
output wire s_axi_AXILiteS_WREADY;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 s_axi_AXILiteS BRESP" *)
output wire [1 : 0] s_axi_AXILiteS_BRESP;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 s_axi_AXILiteS BVALID" *)
output wire s_axi_AXILiteS_BVALID;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 s_axi_AXILiteS BREADY" *)
input wire s_axi_AXILiteS_BREADY;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 s_axi_AXILiteS ARADDR" *)
input wire [4 : 0] s_axi_AXILiteS_ARADDR;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 s_axi_AXILiteS ARVALID" *)
input wire s_axi_AXILiteS_ARVALID;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 s_axi_AXILiteS ARREADY" *)
output wire s_axi_AXILiteS_ARREADY;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 s_axi_AXILiteS RDATA" *)
output wire [31 : 0] s_axi_AXILiteS_RDATA;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 s_axi_AXILiteS RRESP" *)
output wire [1 : 0] s_axi_AXILiteS_RRESP;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 s_axi_AXILiteS RVALID" *)
output wire s_axi_AXILiteS_RVALID;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 s_axi_AXILiteS RREADY" *)
input wire s_axi_AXILiteS_RREADY;
(* X_INTERFACE_INFO = "xilinx.com:signal:clock:1.0 ap_clk CLK" *)
input wire ap_clk;
(* X_INTERFACE_INFO = "xilinx.com:signal:reset:1.0 ap_rst_n RST" *)
input wire ap_rst_n;
(* X_INTERFACE_INFO = "xilinx.com:signal:interrupt:1.0 interrupt INTERRUPT" *)
output wire interrupt;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn AWADDR" *)
output wire [31 : 0] m_axi_WeightsIn_AWADDR;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn AWLEN" *)
output wire [7 : 0] m_axi_WeightsIn_AWLEN;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn AWSIZE" *)
output wire [2 : 0] m_axi_WeightsIn_AWSIZE;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn AWBURST" *)
output wire [1 : 0] m_axi_WeightsIn_AWBURST;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn AWLOCK" *)
output wire [1 : 0] m_axi_WeightsIn_AWLOCK;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn AWREGION" *)
output wire [3 : 0] m_axi_WeightsIn_AWREGION;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn AWCACHE" *)
output wire [3 : 0] m_axi_WeightsIn_AWCACHE;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn AWPROT" *)
output wire [2 : 0] m_axi_WeightsIn_AWPROT;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn AWQOS" *)
output wire [3 : 0] m_axi_WeightsIn_AWQOS;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn AWVALID" *)
output wire m_axi_WeightsIn_AWVALID;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn AWREADY" *)
input wire m_axi_WeightsIn_AWREADY;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn WDATA" *)
output wire [31 : 0] m_axi_WeightsIn_WDATA;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn WSTRB" *)
output wire [3 : 0] m_axi_WeightsIn_WSTRB;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn WLAST" *)
output wire m_axi_WeightsIn_WLAST;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn WVALID" *)
output wire m_axi_WeightsIn_WVALID;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn WREADY" *)
input wire m_axi_WeightsIn_WREADY;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn BRESP" *)
input wire [1 : 0] m_axi_WeightsIn_BRESP;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn BVALID" *)
input wire m_axi_WeightsIn_BVALID;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn BREADY" *)
output wire m_axi_WeightsIn_BREADY;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn ARADDR" *)
output wire [31 : 0] m_axi_WeightsIn_ARADDR;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn ARLEN" *)
output wire [7 : 0] m_axi_WeightsIn_ARLEN;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn ARSIZE" *)
output wire [2 : 0] m_axi_WeightsIn_ARSIZE;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn ARBURST" *)
output wire [1 : 0] m_axi_WeightsIn_ARBURST;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn ARLOCK" *)
output wire [1 : 0] m_axi_WeightsIn_ARLOCK;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn ARREGION" *)
output wire [3 : 0] m_axi_WeightsIn_ARREGION;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn ARCACHE" *)
output wire [3 : 0] m_axi_WeightsIn_ARCACHE;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn ARPROT" *)
output wire [2 : 0] m_axi_WeightsIn_ARPROT;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn ARQOS" *)
output wire [3 : 0] m_axi_WeightsIn_ARQOS;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn ARVALID" *)
output wire m_axi_WeightsIn_ARVALID;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn ARREADY" *)
input wire m_axi_WeightsIn_ARREADY;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn RDATA" *)
input wire [31 : 0] m_axi_WeightsIn_RDATA;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn RRESP" *)
input wire [1 : 0] m_axi_WeightsIn_RRESP;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn RLAST" *)
input wire m_axi_WeightsIn_RLAST;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn RVALID" *)
input wire m_axi_WeightsIn_RVALID;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 m_axi_WeightsIn RREADY" *)
output wire m_axi_WeightsIn_RREADY;
(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 input_V TVALID" *)
input wire input_V_TVALID;
(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 input_V TREADY" *)
output wire input_V_TREADY;
(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 input_V TDATA" *)
input wire [31 : 0] input_V_TDATA;
(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 outputStream_V TVALID" *)
output wire outputStream_V_TVALID;
(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 outputStream_V TREADY" *)
input wire outputStream_V_TREADY;
(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 outputStream_V TDATA" *)
output wire [31 : 0] outputStream_V_TDATA;

  convHW #(
    .C_S_AXI_AXILITES_ADDR_WIDTH(5),
    .C_S_AXI_AXILITES_DATA_WIDTH(32),
    .C_M_AXI_WEIGHTSIN_ID_WIDTH(1),
    .C_M_AXI_WEIGHTSIN_ADDR_WIDTH(32),
    .C_M_AXI_WEIGHTSIN_DATA_WIDTH(32),
    .C_M_AXI_WEIGHTSIN_AWUSER_WIDTH(1),
    .C_M_AXI_WEIGHTSIN_ARUSER_WIDTH(1),
    .C_M_AXI_WEIGHTSIN_WUSER_WIDTH(1),
    .C_M_AXI_WEIGHTSIN_RUSER_WIDTH(1),
    .C_M_AXI_WEIGHTSIN_BUSER_WIDTH(1),
    .C_M_AXI_WEIGHTSIN_USER_VALUE('H00000000),
    .C_M_AXI_WEIGHTSIN_PROT_VALUE('B000),
    .C_M_AXI_WEIGHTSIN_CACHE_VALUE('B0011)
  ) inst (
    .s_axi_AXILiteS_AWADDR(s_axi_AXILiteS_AWADDR),
    .s_axi_AXILiteS_AWVALID(s_axi_AXILiteS_AWVALID),
    .s_axi_AXILiteS_AWREADY(s_axi_AXILiteS_AWREADY),
    .s_axi_AXILiteS_WDATA(s_axi_AXILiteS_WDATA),
    .s_axi_AXILiteS_WSTRB(s_axi_AXILiteS_WSTRB),
    .s_axi_AXILiteS_WVALID(s_axi_AXILiteS_WVALID),
    .s_axi_AXILiteS_WREADY(s_axi_AXILiteS_WREADY),
    .s_axi_AXILiteS_BRESP(s_axi_AXILiteS_BRESP),
    .s_axi_AXILiteS_BVALID(s_axi_AXILiteS_BVALID),
    .s_axi_AXILiteS_BREADY(s_axi_AXILiteS_BREADY),
    .s_axi_AXILiteS_ARADDR(s_axi_AXILiteS_ARADDR),
    .s_axi_AXILiteS_ARVALID(s_axi_AXILiteS_ARVALID),
    .s_axi_AXILiteS_ARREADY(s_axi_AXILiteS_ARREADY),
    .s_axi_AXILiteS_RDATA(s_axi_AXILiteS_RDATA),
    .s_axi_AXILiteS_RRESP(s_axi_AXILiteS_RRESP),
    .s_axi_AXILiteS_RVALID(s_axi_AXILiteS_RVALID),
    .s_axi_AXILiteS_RREADY(s_axi_AXILiteS_RREADY),
    .ap_clk(ap_clk),
    .ap_rst_n(ap_rst_n),
    .interrupt(interrupt),
    .m_axi_WeightsIn_AWID(),
    .m_axi_WeightsIn_AWADDR(m_axi_WeightsIn_AWADDR),
    .m_axi_WeightsIn_AWLEN(m_axi_WeightsIn_AWLEN),
    .m_axi_WeightsIn_AWSIZE(m_axi_WeightsIn_AWSIZE),
    .m_axi_WeightsIn_AWBURST(m_axi_WeightsIn_AWBURST),
    .m_axi_WeightsIn_AWLOCK(m_axi_WeightsIn_AWLOCK),
    .m_axi_WeightsIn_AWREGION(m_axi_WeightsIn_AWREGION),
    .m_axi_WeightsIn_AWCACHE(m_axi_WeightsIn_AWCACHE),
    .m_axi_WeightsIn_AWPROT(m_axi_WeightsIn_AWPROT),
    .m_axi_WeightsIn_AWQOS(m_axi_WeightsIn_AWQOS),
    .m_axi_WeightsIn_AWUSER(),
    .m_axi_WeightsIn_AWVALID(m_axi_WeightsIn_AWVALID),
    .m_axi_WeightsIn_AWREADY(m_axi_WeightsIn_AWREADY),
    .m_axi_WeightsIn_WID(),
    .m_axi_WeightsIn_WDATA(m_axi_WeightsIn_WDATA),
    .m_axi_WeightsIn_WSTRB(m_axi_WeightsIn_WSTRB),
    .m_axi_WeightsIn_WLAST(m_axi_WeightsIn_WLAST),
    .m_axi_WeightsIn_WUSER(),
    .m_axi_WeightsIn_WVALID(m_axi_WeightsIn_WVALID),
    .m_axi_WeightsIn_WREADY(m_axi_WeightsIn_WREADY),
    .m_axi_WeightsIn_BID(1'B0),
    .m_axi_WeightsIn_BRESP(m_axi_WeightsIn_BRESP),
    .m_axi_WeightsIn_BUSER(1'B0),
    .m_axi_WeightsIn_BVALID(m_axi_WeightsIn_BVALID),
    .m_axi_WeightsIn_BREADY(m_axi_WeightsIn_BREADY),
    .m_axi_WeightsIn_ARID(),
    .m_axi_WeightsIn_ARADDR(m_axi_WeightsIn_ARADDR),
    .m_axi_WeightsIn_ARLEN(m_axi_WeightsIn_ARLEN),
    .m_axi_WeightsIn_ARSIZE(m_axi_WeightsIn_ARSIZE),
    .m_axi_WeightsIn_ARBURST(m_axi_WeightsIn_ARBURST),
    .m_axi_WeightsIn_ARLOCK(m_axi_WeightsIn_ARLOCK),
    .m_axi_WeightsIn_ARREGION(m_axi_WeightsIn_ARREGION),
    .m_axi_WeightsIn_ARCACHE(m_axi_WeightsIn_ARCACHE),
    .m_axi_WeightsIn_ARPROT(m_axi_WeightsIn_ARPROT),
    .m_axi_WeightsIn_ARQOS(m_axi_WeightsIn_ARQOS),
    .m_axi_WeightsIn_ARUSER(),
    .m_axi_WeightsIn_ARVALID(m_axi_WeightsIn_ARVALID),
    .m_axi_WeightsIn_ARREADY(m_axi_WeightsIn_ARREADY),
    .m_axi_WeightsIn_RID(1'B0),
    .m_axi_WeightsIn_RDATA(m_axi_WeightsIn_RDATA),
    .m_axi_WeightsIn_RRESP(m_axi_WeightsIn_RRESP),
    .m_axi_WeightsIn_RLAST(m_axi_WeightsIn_RLAST),
    .m_axi_WeightsIn_RUSER(1'B0),
    .m_axi_WeightsIn_RVALID(m_axi_WeightsIn_RVALID),
    .m_axi_WeightsIn_RREADY(m_axi_WeightsIn_RREADY),
    .input_V_TVALID(input_V_TVALID),
    .input_V_TREADY(input_V_TREADY),
    .input_V_TDATA(input_V_TDATA),
    .outputStream_V_TVALID(outputStream_V_TVALID),
    .outputStream_V_TREADY(outputStream_V_TREADY),
    .outputStream_V_TDATA(outputStream_V_TDATA)
  );
endmodule
