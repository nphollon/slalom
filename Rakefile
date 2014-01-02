COMPILER = "clang++"

task :default => [:test]

task :test => ["runtests"] do
  sh "./runtests"
end

file "runtests" => ["lambda_test.o","tester.o","parse.o","node.o","strutil.o"] do |target|
  sh "#{COMPILER} -o #{target.name} #{target.prerequisites.join(' ')}"
end

rule ".o" => ".cpp" do |target|
  sh "#{COMPILER} -c -o #{target.name} #{target.source}"
end
