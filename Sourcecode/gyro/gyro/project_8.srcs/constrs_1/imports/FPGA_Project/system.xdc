## Clock signal 100 MHz
set_property -dict { PACKAGE_PIN H4  IOSTANDARD LVCMOS33 } [get_ports { i_clk }]; #IO_L13P_T2_MRCC_35 Sch=sysclk
create_clock -add -name sys_clk_pin -period 10.00 -waveform {0 5} [get_ports { i_clk }];

#UART0
#set_property -dict {PACKAGE_PIN A3 IOSTANDARD LVCMOS33} [get_ports UART0_Rx];
#set_property -dict {PACKAGE_PIN A5 IOSTANDARD LVCMOS33} [get_ports UART0_Tx];
#Rst
set_property -dict {PACKAGE_PIN D14 IOSTANDARD LVCMOS33} [get_ports i_rst];

#KEY
set_property -dict {PACKAGE_PIN C3 IOSTANDARD LVCMOS33} [get_ports i_en];

#EN
set_property -dict {PACKAGE_PIN M3 IOSTANDARD LVCMOS33} [get_ports {o_gpio_en[0]}];
set_property -dict {PACKAGE_PIN N4 IOSTANDARD LVCMOS33} [get_ports {o_gpio_en[1]}];
set_property -dict {PACKAGE_PIN L13 IOSTANDARD LVCMOS33} [get_ports o_src_en];
#Gyro
set_property -dict {PACKAGE_PIN P12 IOSTANDARD LVCMOS33} [get_ports o_iic_scl];
set_property -dict {PACKAGE_PIN P13 IOSTANDARD LVCMOS33} [get_ports i_iic_sda];

set_property PULLUP true [get_ports {o_iic_scl}]
set_property PULLUP true [get_ports {i_iic_sda}]

#QSPI
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {I_qspi_clk_IBUF}]
set_property -dict {PACKAGE_PIN P2 IOSTANDARD LVCMOS33}  [get_ports {qspi_d0}]
set_property -dict {PACKAGE_PIN L14 IOSTANDARD LVCMOS33} [get_ports {qspi_d1}]
set_property -dict {PACKAGE_PIN J13 IOSTANDARD LVCMOS33} [get_ports {qspi_d2}]
set_property -dict {PACKAGE_PIN D13 IOSTANDARD LVCMOS33} [get_ports {qspi_d3}]
set_property -dict {PACKAGE_PIN M13 IOSTANDARD LVCMOS33} [get_ports {I_qspi_cs}]
set_property -dict {PACKAGE_PIN H14 IOSTANDARD LVCMOS33} [get_ports {I_qspi_clk}]

set_property SEVERITY {Warning} [get_drc_checks NSTD-1]
set_property SEVERITY {Warning} [get_drc_checks UCIO-1]


set_property CONFIG_VOLTAGE 3.3 [current_design]
set_property CFGBVS VCCO [current_design]
set_property BITSTREAM.CONFIG.CONFIGRATE 50 [current_design]