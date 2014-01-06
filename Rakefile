COMPILER = "clang++"
AS_FLAGS = "`llvm-config --cxxflags`"
LINK_FLAGS = "`llvm-config --cxxflags --libs core` `llvm-config --ldflags`"
OBJECTS = ["parse.o", "node.o", "strutil.o"]

def compile(target)
  sh "#{COMPILER} #{target.prerequisites.join(' ')} #{LINK_FLAGS} -o #{target.name}"
end

task :default => [:all]

task :all => [:test, "repl", "codegen"]

task :test => ["runtests"] do
  sh "./runtests"
end

file "repl" => ["slalom_repl.o"] + OBJECTS do |target|
  compile target
end

file "runtests" => ["slalom_test.o","tester.o"] + OBJECTS do |target|
  compile target
end

file "codegen" => ["codegen.o"] + OBJECTS do |target|
  compile target
end

rule ".o" => ".cpp" do |target|
  sh "#{COMPILER} #{AS_FLAGS} -c -o #{target.name} #{target.source}"
end

task :clean do
  sh "rm -f *.o repl runtests"
end
