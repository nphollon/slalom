COMPILER = "clang++"
AS_FLAGS = "`llvm-config --cxxflags`"
LINK_FLAGS = "`llvm-config --cxxflags --libs engine` `llvm-config --ldflags`"
OBJECTS = ["parse.o", "node.o", "strutil.o", "codegenerator.o"]

def compile(target)
  sh "#{COMPILER} #{target.prerequisites.join(' ')} #{LINK_FLAGS} -o #{target.name}"
end

task :default => [:all]

task :all => [:test, :repl, :codegen]

task :test => ["bin", "bin/runtests"] do
  sh "./bin/runtests"
end

task :repl => ["bin", "bin/repl"]
task :codegen => ["bin", "bin/codegen"]

task :clean do
  sh "rm -rf *.o bin"
end

directory "bin"

file "bin/repl" => ["slalom_repl.o"] + OBJECTS do |target|
  compile target
end

file "bin/runtests" => ["slalom_test.o", "tester.o"] + OBJECTS do |target|
  compile target
end

file "bin/codegen" => ["codegen_spike.o"] + OBJECTS do |target|
  compile target
end

rule ".o" => ".cpp" do |target|
  sh "#{COMPILER} #{AS_FLAGS} -c -o #{target.name} #{target.source}"
end
