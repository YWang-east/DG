import os
import pandas as pd
import matplotlib.pyplot as plt

plt.rcParams['text.usetex'] = True
plt.rcParams["font.family"] = "Times New Roman"
bwidth = 2.0
font_size  = 15
label_size = 15
marker_size= 3
line_width = 1.2
legend_size= 13

#--------------------------------------------------------------------------#
# data
#--------------------------------------------------------------------------#

case_path = 'build/examples/'
case_name1 = 'p_adaptivity'
case_name2 = 'h_adaptivity'

parent_dir = os.path.join(os.path.dirname(__file__), os.pardir)
file_path1 = os.path.join(parent_dir, case_path + case_name1 + '.csv')
file_path2 = os.path.join(parent_dir, case_path + case_name2 + '.csv')

data1 = pd.read_csv(file_path1)
data2 = pd.read_csv(file_path2)

x1   = data1['p']
var1 = data1['error']

x2   = data2['n']
var2 = data2['error']
#--------------------------------------------------------------------------#
# plot
#--------------------------------------------------------------------------#
fig, ax = plt.subplots()
ax.plot(x1, var1, 'ro', markersize = marker_size, label=r'$p$ - adaptivity')
ax.plot(x2, var2, 'ko', markersize = marker_size, label=r'$h$ - adaptivity')

ax.spines['top'].set_linewidth(bwidth)
ax.spines['bottom'].set_linewidth(bwidth)
ax.spines['left'].set_linewidth(bwidth)
ax.spines['right'].set_linewidth(bwidth)
ax.axes.xaxis.set_ticks_position('both')
ax.tick_params(axis='x', direction='in', labelsize=label_size)
ax.set_xlabel(r'degree of freedom', fontsize=font_size)
ax.set_xlim(0, 25)
ax.set_yscale('log')
plt.minorticks_off()
ax.axes.yaxis.set_ticks_position('both')
ax.tick_params(axis='y', direction='in', labelsize=label_size)
ax.set_ylabel(r'$L_2$ error', fontsize=font_size, fontweight = 'bold')
ax.legend(loc='lower left',frameon=False,fontsize=legend_size)

plt.show()
# save plot to pdf
fig.savefig('vis/convergence.pdf', bbox_inches='tight')


