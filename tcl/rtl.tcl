set prj_file [pwd]/rtl/rtl.xpr

open_project $prj_file 

launch_runs -jobs 8 impl_1 -to_step write_bitstream
wait_on_run impl_1

close_project

exit
