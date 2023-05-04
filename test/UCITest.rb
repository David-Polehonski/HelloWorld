require 'test/unit'

class UCITest < Test::Unit::TestCase

  def test_uciRespondsWithID
    response = ""
    executable = File.expand_path("./HelloWorld.exe");
    if(File.exist? executable)
      IO.popen(executable, "w+", :external_encoding=>Encoding::ASCII_8BIT) do |engine|
        engine.write("uci\n")
        engine.flush
        sleep(1)
        response = engine.readpartial(1024)
        engine.write("quit\n")
        engine.flush
      end

      assert_include(response, "id name HelloWorld");
      assert_include(response, "id author David Polehonski");
      assert_include(response, "uciok");
    end
  end


end