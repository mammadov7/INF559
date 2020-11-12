#!/usr//bin/python
#
# driver.py - The driver tests the correctness of the student's cache
#     simulator and the correctness and performance of their rotate
#     function. It uses ./test-csim to check the correctness of the
#     simulator and it runs ./test-rotate on three different sized
#     matrices (32x32, 64x64, and 67x67) to test the correctness and
#     performance of the rotate function.
#
import subprocess;
import re;
import os;
import sys;
import optparse;

#
# computeMissScore - compute the score depending on the number of
# cache misses
#
def computeMissScore(miss, lower, upper, full_score):
    if miss <= lower:
        return full_score
    if miss >= upper: 
        return 0

    score = (miss - lower) * 1.0 
    range = (upper- lower) * 1.0
    return round((1 - score / range) * full_score, 1)

#
# main - Main function
#
def main():

    # Configure maxscores here
    maxscore= {};
    maxscore['csim'] = 27
    maxscore['rotatec'] = 1
    maxscore['rotate32'] = 8
    maxscore['rotate64'] = 8
    maxscore['rotate67'] = 10

    # Parse the command line arguments 
    p = optparse.OptionParser()
    p.add_option("-A", action="store_true", dest="autograde", 
                 help="emit autoresult string for Autolab");
    opts, args = p.parse_args()
    autograde = opts.autograde

    # Check the correctness of the cache simulator
    print "Part A: Testing cache simulator"
    print "Running ./test-csim"
    p = subprocess.Popen("./test-csim", 
                         shell=True, stdout=subprocess.PIPE)
    stdout_data = p.communicate()[0]

    # Emit the output from test-csim
    stdout_data = re.split('\n', stdout_data)
    for line in stdout_data:
        if re.match("TEST_CSIM_RESULTS", line):
            resultsim = re.findall(r'(\d+)', line)
        else:
            print "%s" % (line)

    # Check the correctness and performance of the rotate function
    # 32x32 rotate
    print "Part B: Testing rotate function"
    print "Running ./test-rotate -N 32"
    p = subprocess.Popen("./test-rotate -N 32 | grep TEST_ROTATE_RESULTS", 
                         shell=True, stdout=subprocess.PIPE)
    stdout_data = p.communicate()[0]
    result32 = re.findall(r'(\d+)', stdout_data)
    
    # 64x64 rotate
    print "Running ./test-rotate -N 64"
    p = subprocess.Popen("./test-rotate -N 64 | grep TEST_ROTATE_RESULTS", 
                         shell=True, stdout=subprocess.PIPE)
    stdout_data = p.communicate()[0]
    result64 = re.findall(r'(\d+)', stdout_data)
    
    # 67x67 rotate
    print "Running ./test-rotate -N 67"
    p = subprocess.Popen("./test-rotate -N 67 | grep TEST_ROTATE_RESULTS", 
                         shell=True, stdout=subprocess.PIPE)
    stdout_data = p.communicate()[0]
    result67 = re.findall(r'(\d+)', stdout_data)
    
    # Compute the scores for each step
    csim_cscore  = map(int, resultsim[0:1])
    rotate_cscore = int(result32[0]) * int(result64[0]) * int(result67[0]);
    miss32 = int(result32[1])
    miss64 = int(result64[1])
    miss67 = int(result67[1])
    rotate32_score = computeMissScore(miss32, 300, 600, maxscore['rotate32']) * int(result32[0])
    rotate64_score = computeMissScore(miss64, 1300, 2000, maxscore['rotate64']) * int(result64[0])
    rotate67_score = computeMissScore(miss67, 2000, 3000, maxscore['rotate67']) * int(result67[0])
    total_score = csim_cscore[0] + rotate32_score + rotate64_score + rotate67_score

    # Summarize the results
    print "\nCache Lab summary:"
    print "%-22s%8s%10s%12s" % ("", "Points", "Max pts", "Misses")
    print "%-22s%8.1f%10d" % ("Csim correctness", csim_cscore[0], 
                              maxscore['csim'])

    misses = str(miss32)
    if miss32 == 2**31-1 :
        misses = "invalid"
    print "%-22s%8.1f%10d%12s" % ("Rotate perf 32x32", rotate32_score, 
                                  maxscore['rotate32'], misses)

    misses = str(miss64)
    if miss64 == 2**31-1 :
        misses = "invalid"
    print "%-22s%8.1f%10d%12s" % ("Rotate perf 64x64", rotate64_score, 
                                  maxscore['rotate64'], misses)

    misses = str(miss67)
    if miss67 == 2**31-1 :
        misses = "invalid"
    print "%-22s%8.1f%10d%12s" % ("Rotate perf 67x67", rotate67_score, 
                                  maxscore['rotate67'], misses)

    print "%22s%8.1f%10d" % ("Total points", total_score,
                             maxscore['csim'] + 
                             maxscore['rotate32'] + 
                             maxscore['rotate64'] +
                             maxscore['rotate67'])
    
    # Emit autoresult string for Autolab if called with -A option
    if autograde:
        autoresult="%.1f:%d:%d:%d" % (total_score, miss32, miss64, miss67)
        print "\nAUTORESULT_STRING=%s" % autoresult
    
    
# execute main only if called as a script
if __name__ == "__main__":
    main()

