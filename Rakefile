COMPILER = "clang++"
BOOST_ROOT = ENV["BOOST_ROOT"] || "/usr/local"
AS_FLAGS = "-c `llvm-config --cxxflags`"
LINK_FLAGS = "`llvm-config --cxxflags --libs engine` `llvm-config --ldflags`"
TEST_FLAGS = AS_FLAGS + " -frtti -fexceptions -I #{BOOST_ROOT}/boost_1_55_0"

OBJECTS = ["src/generator.o",
           "src/node.o",
           "src/program_text.o",
           "src/ir_module_writer.o",
           "src/ir_slalom_function.o",
           "src/ir_arguments_queue.o",
           "src/ir_queue_node.o"
          ]

TEST_OBJECTS = OBJECTS + [
                          "test/test_main.o",
                          "test/name_node_test.o",
                          "test/apply_node_test.o",
                          "test/parse_test.o",
                          "test/generator_test.o",
                          "test/module_test.o",
                          "test/test_jit.o",
                          "#{BOOST_ROOT}/lib/libboost_unit_test_framework.a"
                         ]

def objects_in_directory(dir)
  # regex matches files in dir/ ending in .o
  /#{dir}\/.*\.o$/
end

def source_file_for_object
  proc { |f| f.sub(/\.o$/, ".cpp") }
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

file "bin/runtests" => TEST_OBJECTS do |target|
  compile target, LINK_FLAGS
end

rule objects_in_directory("test") => [ source_file_for_object ] do |target|
  compile target, TEST_FLAGS
end

rule objects_in_directory("src") => [ source_file_for_object ] do |target|
  compile target, AS_FLAGS
end

def compile(target, flags)
  sh "#{COMPILER} #{target.prerequisites.join(' ')} #{flags} -o #{target.name}"
end
