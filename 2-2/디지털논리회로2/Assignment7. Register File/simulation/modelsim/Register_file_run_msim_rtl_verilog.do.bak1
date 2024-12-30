transcript on
if {[file exists rtl_work]} {
	vdel -lib rtl_work -all
}
vlib rtl_work
vmap work rtl_work

vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/Assignment7.\ Register\ File {C:/Users/kk200/verilog/Assignment7. Register File/Register_file.v}
vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/Assignment7.\ Register\ File {C:/Users/kk200/verilog/Assignment7. Register File/register32_r_en.v}
vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/Assignment7.\ Register\ File {C:/Users/kk200/verilog/Assignment7. Register File/write_operation.v}
vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/Assignment7.\ Register\ File {C:/Users/kk200/verilog/Assignment7. Register File/read_operation.v}
vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/Assignment7.\ Register\ File {C:/Users/kk200/verilog/Assignment7. Register File/register32_8.v}
vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/Assignment7.\ Register\ File {C:/Users/kk200/verilog/Assignment7. Register File/_3_to_8_decoder.v}
vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/Assignment7.\ Register\ File {C:/Users/kk200/verilog/Assignment7. Register File/_8_to_1_MUX.v}

vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/Assignment7.\ Register\ File {C:/Users/kk200/verilog/Assignment7. Register File/tb_Register_file.v}

vsim -t 1ps -L altera_ver -L lpm_ver -L sgate_ver -L altera_mf_ver -L altera_lnsim_ver -L cyclonev_ver -L cyclonev_hssi_ver -L cyclonev_pcie_hip_ver -L rtl_work -L work -voptargs="+acc"  tb_Register_file

add wave *
view structure
view signals
run -all
