import numpy as np
import pandas as pd
import xlrd

file_location = "C:\\Users\\Administrator\\Desktop\\数据.xlsx"
data = xlrd.open_workbook(file_location)

df = pd.DataFrame(np.random.randn(3, 10), )
