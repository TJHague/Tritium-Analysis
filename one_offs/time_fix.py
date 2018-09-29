import sys
import os
import string
import MySQLdb
import subprocess


#Returns the value of the requested EPICS variable by interfacing with caget (a standalone script on adaq)
def timestamp(run):
  query = ['analyzer','-l','\'getTime('+str(run)+')\'']
  time = subprocess.Popen(query, stdout=subprocess.PIPE, stderr=subprocess.PIPE, stdin=subprocess.PIPE) #runs analyzer script
  ret, err = time.communicate()
  return ret.splitlines()[2].rstrip()

old = open("db_run.dat","r")
new = open("new_db_run.dat","w")

while True: #not quite proper python, but need option of reading more than one line at a time... This will get the job done for a one-time use script
  line = old.readline()
  if not line:
    break
  elif line.startswith('-'):
    run_line = old.readline()
    run = run_line[28:].rstrip()
    new.write(timestamp(run))
    new.write(run_line)
  else:
    new.write(line)


