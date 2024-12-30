transcript on
if {[file exists rtl_work]} {
	vdel -lib rtl_work -all
}
vlib rtl_work
vmap work rtl_work

vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/week5/_dff_r_sync_async {C:/Users/kk200/verilog/week5/_dff_r_sync_async/_dff_r_sync.v}
vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/week5/_dff_r_sync_async {C:/Users/kk200/verilog/week5/_dff_r_sync_async/_dff_r_async.v}
vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/week5/_dff_r_sync_async {C:/Users/kk200/verilog/week5/_dff_r_sync_async/_dff_r_sync_async.v}

vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/week5/_dff_r_sync_async {C:/Users/kk200/verilog/week5/_dff_r_sync_async/tb_dff_r_sync_async.v}

vsim -t 1ps -L altera_ver -L lpm_ver -L sgate_ver -L altera_mf_ver -L altera_lnsim_ver -L cyclonev_ver -L cyclonev_hssi_ver -L cyclonev_pcie_hip_ver -L rtl_work -L work -voptargs="+acc"  tb_dff_r_sync_async

add wave *
view structure
view signals
run -all
