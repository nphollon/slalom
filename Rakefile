COMPILER = "clang++"
AS_FLAGS = "-c `llvm-config --cxxflags`"
LINK_FLAGS = "`llvm-config --cxxflags --libs engine` `llvm-config --ldflags`"
TEST_FLAGS = AS_FLAGS + " -frtti -fexceptions -I /usr/local/boost_1_55_0"

OBJECTS = ["src/generate.o",
           "src/parse.o",
           "src/node.o",
           "src/strutil.o"]

TEST_OBJECTS = ["test/test_main.o",
                "test/name_node_test.o",
                "test/apply_node_test.o",
                "test/parse_test.o",
                "test/generate_test.o"
              ]

def compile(target, flags)
  sh "#{COMPILER} #{target.prerequisites.join(' ')} #{flags} -o #{target.name}"
end

task :default => [:all]

task :all => [:test, :repl]

task :test => ["bin", "bin/runtests"] do
  sh "./bin/runtests"
end

task :repl => ["bin", "bin/repl"]

task :clean do
  sh "rm -rf *.o */*.o bin"
end

directory "bin"

file "bin/repl" => ["src/repl.o"] + OBJECTS do |target|
  compile target, LINK_FLAGS
end

file "bin/runtests" => TEST_OBJECTS + OBJECTS do |target|
  compile target, LINK_FLAGS
end

rule /test\/.*\.o$/ => [proc {|f| f.sub(/\.o$/, '.cpp') }] do |target|
  compile target, TEST_FLAGS
end

rule /src\/.*\.o$/ => [proc {|f| f.sub(/\.o$/, '.cpp') }] do |target|
  compile target, AS_FLAGS
end
