transcript on
if {[file exists rtl_work]} {
	vdel -lib rtl_work -all
}
vlib rtl_work
vmap work rtl_work

vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/CLA_CLK {C:/Users/kk200/verilog/CLA_CLK/rca32.v}
vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/CLA_CLK {C:/Users/kk200/verilog/CLA_CLK/rca4.v}
vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/CLA_CLK {C:/Users/kk200/verilog/CLA_CLK/rca_clk.v}
vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/CLA_CLK {C:/Users/kk200/verilog/CLA_CLK/ha.v}
vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/CLA_CLK {C:/Users/kk200/verilog/CLA_CLK/fa.v}
vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/CLA_CLK {C:/Users/kk200/verilog/CLA_CLK/gates.v}

vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/CLA_CLK {C:/Users/kk200/verilog/CLA_CLK/tb_rca_clk.v}

vsim -t 1ps -L altera_ver -L lpm_ver -L sgate_ver -L altera_mf_ver -L altera_lnsim_ver -L cyclonev_ver -L cyclonev_hssi_ver -L cyclonev_pcie_hip_ver -L rtl_work -L work -voptargs="+acc"  tb_rca_clk

add wave *
view structure
view signals
run -all
