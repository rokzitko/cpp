import sys
sys.path.append("") # current directory
import my_ext
#help(my_ext)
print(my_ext.add(a=1,b=2)==3)
print(my_ext.the_answer)
d=my_ext.el(42,"rok")
print(d)
print(d.get_value())
print(d.get_string())
print(d.value)
print(d.str)

print(my_ext.double_it([1, 2, 3]))
my_ext.dump([d,d,d])
