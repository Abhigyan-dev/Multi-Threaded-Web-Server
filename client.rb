require 'socket'
starttime = Process.clock_gettime(Process::CLOCK_MONOTONIC)
s = TCPSocket.new 'localhost', 5000

s.write("GET ./data/#{ARGV[0]}.html HTTP/1.1\n")

s.each_line do |line|

# puts line # uncomment if you want to print output

end
 
s.close
endtime = Process.clock_gettime(Process::CLOCK_MONOTONIC)
elapsed_time = endtime - starttime
puts "Elapsed: #{elapsed_time}"