cmd_/home/ggumnet/kernellab-handout/paddr/Module.symvers := sed 's/\.ko$$/\.o/' /home/ggumnet/kernellab-handout/paddr/modules.order | scripts/mod/modpost -m -a  -o /home/ggumnet/kernellab-handout/paddr/Module.symvers -e -i Module.symvers   -T -
