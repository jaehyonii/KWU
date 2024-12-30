onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate /tb_fifo/tb_clk
add wave -noupdate /tb_fifo/tb_reset_n
add wave -noupdate /tb_fifo/tb_rd_en
add wave -noupdate /tb_fifo/tb_wr_en
add wave -noupdate -radix hexadecimal /tb_fifo/tb_d_in
add wave -noupdate -radix hexadecimal /tb_fifo/tb_data_count
add wave -noupdate -radix hexadecimal /tb_fifo/tb_d_out
add wave -noupdate /tb_fifo/tb_full
add wave -noupdate /tb_fifo/tb_empty
add wave -noupdate /tb_fifo/tb_wr_ack
add wave -noupdate /tb_fifo/tb_wr_err
add wave -noupdate /tb_fifo/tb_rd_ack
add wave -noupdate /tb_fifo/tb_rd_err
add wave -noupdate /tb_fifo/U0/state
add wave -noupdate /tb_fifo/U0/next_state
add wave -noupdate /tb_fifo/U0/head
add wave -noupdate /tb_fifo/U0/tail
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {87906 ps} 0}
quietly wave cursor active 1
configure wave -namecolwidth 150
configure wave -valuecolwidth 100
configure wave -justifyvalue left
configure wave -signalnamewidth 0
configure wave -snapdistance 10
configure wave -datasetprefix 0
configure wave -rowmargin 4
configure wave -childrowmargin 2
configure wave -gridoffset 0
configure wave -gridperiod 1
configure wave -griddelta 40
configure wave -timeline 0
configure wave -timelineunits ps
update
WaveRestoreZoom {0 ps} {339150 ps}
