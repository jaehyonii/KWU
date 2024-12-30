transcript on
if {[file exists rtl_work]} {
	vdel -lib rtl_work -all
}
vlib rtl_work
vmap work rtl_work

vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/Assignment8.\ FIFO {C:/Users/kk200/verilog/Assignment8. FIFO/register32_8.v}
vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/Assignment8.\ FIFO {C:/Users/kk200/verilog/Assignment8. FIFO/_8_to_1_MUX.v}
vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/Assignment8.\ FIFO {C:/Users/kk200/verilog/Assignment8. FIFO/_3_to_8_decoder.v}
vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/Assignment8.\ FIFO {C:/Users/kk200/verilog/Assignment8. FIFO/write_operation.v}
vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/Assignment8.\ FIFO {C:/Users/kk200/verilog/Assignment8. FIFO/register32_r_en.v}
vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/Assignment8.\ FIFO {C:/Users/kk200/verilog/Assignment8. FIFO/read_operation.v}
vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/Assignment8.\ FIFO {C:/Users/kk200/verilog/Assignment8. FIFO/fifo.v}
vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/Assignment8.\ FIFO {C:/Users/kk200/verilog/Assignment8. FIFO/fifo_ns.v}
vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/Assignment8.\ FIFO {C:/Users/kk200/verilog/Assignment8. FIFO/fifo_cal.v}
vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/Assignment8.\ FIFO {C:/Users/kk200/verilog/Assignment8. FIFO/fifo_out.v}
vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/Assignment8.\ FIFO {C:/Users/kk200/verilog/Assignment8. FIFO/Register_file.v}
vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/Assignment8.\ FIFO {C:/Users/kk200/verilog/Assignment8. FIFO/register3_r_en.v}
vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/Assignment8.\ FIFO {C:/Users/kk200/verilog/Assignment8. FIFO/register4_r_en.v}

vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/Assignment8.\ FIFO {C:/Users/kk200/verilog/Assignment8. FIFO/tb_fifo.v}

vsim -t 1ps -L altera_ver -L lpm_ver -L sgate_ver -L altera_mf_ver -L altera_lnsim_ver -L cyclonev_ver -L cyclonev_hssi_ver -L cyclonev_pcie_hip_ver -L rtl_work -L work -voptargs="+acc"  tb_fifo

add wave *
view structure
view signals
run -all
