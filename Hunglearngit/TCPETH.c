/*@!Encoding:1252*/
includes
{
  
}

variables
{
  
}


void testcase1()
{
    TcpSocket gtcpSocket;
  
    gtcpSocket = TcpSocket::Open( IP_Endpoint(198.18.34.1:32201) );
    
    if (IpGetLastError() != 0)
    {
      // if UdpSocket::Open fails, we print a message to the write window
      write( "<%BASE_FILE_NAME%> TcpSocket::Open failed with reauls %d", IpGetLastError() );
      return;
    }
    
    // Connect to server
    gtcpSocket.Connect( IP_Endpoint( 198.18.36.96:58097) );
    
    if ((gtcpSocket.GetLastSocketError() != 0) && (gtcpSocket.GetLastSocketError() != 10035))
    {
      char errorString[100分];
      
      // if ReceiveFrom fails, we print a message to the write window
      gtcpSocket.GetLastSocketErrorAsString( errorString, elcount(errorString) );
      write( "<%BASE_FILE_NAME%> TCPSocket::Connect failed with result %d (%s)", gtcpSocket.GetLastSocketError(), errorString );
    }
    
}

MainTest()
{
  testcase1();
}