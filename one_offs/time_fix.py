import sys
import os
import string
import subprocess


#Returns the value of the requested EPICS variable by interfacing with caget (a standalone script on adaq)
def timestamp(run):
  func = '\'getTime.cpp('+str(run)+')\''
  print(func)
#  query = ['analyzer','-l',func]
  query = ['test']
  time = subprocess.Popen(query, stdout=subprocess.PIPE, stderr=subprocess.PIPE, stdin=subprocess.PIPE) #runs analyzer script
  ret, err = time.communicate()
  print(ret)
  print(err)
  return ret.rstrip()

old = open("db_run.dat","r")
new = open("new_db_run.dat","w")

while True: #not quite proper python, but need option of reading more than one line at a time... This will get the job done for a one-time use script
  line = old.readline()
  if not line:
    break
  elif line.startswith('-'):
    run_line = old.readline()
    run = run_line[28:].rstrip()
    time_line = timestamp(run)
    if time_line.startswith('-'):
      new.write(time_line)
    else:
      print(time_line)
      new.write(line) #if something goes wrong, keep the old timestamp
      print(run)
      print('\n')
    new.write(run_line)
  else:
    new.write(line)


