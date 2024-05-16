import chipwhisperer as cw
## Define and program the bitstream
bitstream =	"build/gateware/top.runs/impl_1/top.bit"
# bitstream =	"top.bit"

target = cw.target(None, cw.targets.CW305, bsfile=bitstream)
print("Loaded bitstream")

## Set clock frequency and low noise parameters
target.vccint_set(1.00)

target.pll.pll_outfreq_set(50e6, 1)
# # target.pll.pll_outfreq_set(12.5E6, 1)
# target.pll.pll_outfreq_set(9E6, 0)

target.pll.pll_outsource_set('PLL1',0)
print(target.pll.pll_outsource_get(0))
target.pll.pll_enable_set(True)
target.pll.pll_outenable_set(True, 0)
target.pll.pll_outenable_set(True, 1)
target.pll.pll_outenable_set(False, 2)


target.clkusbautooff = True
target.clksleeptime = 1
