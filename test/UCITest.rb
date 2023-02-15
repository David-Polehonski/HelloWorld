require 'test/unit'

class UCITest < Test::Unit::TestCase

  def readline_nonblock
    buffer = ""
    buffer << read_nonblock(1) while buffer[-1] != "\n"

    buffer
  rescue IO::WaitReadable => blocking
    raise blocking if buffer.empty?

    buffer
  end

  def test_uciRespondsWithID
    response = ""
    if(File.exist? "HelloWorld.exe")
      IO.popen("HelloWorld.exe", "w+", :external_encoding=>Encoding::ASCII_8BIT) do |engine|


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