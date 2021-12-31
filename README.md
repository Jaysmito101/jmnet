<br/>
<p align="center">
    <img src="https://github.com/Jaysmito101/jmnet/blob/main/resources/ReadmeHeader.png?raw=true" border="0"></
</p>

<br/>
<p align="center">
  <img alt="GitHub repo size" src="https://img.shields.io/github/repo-size/Jaysmito101/jmnet?style=for-the-badge">
  <img alt="Lines of code" src="https://img.shields.io/tokei/lines/github/Jaysmito101/jmnet?style=for-the-badge">
  <img alt="GitHub commit activity" src="https://img.shields.io/github/commit-activity/w/Jaysmito101/jmnet?style=for-the-badge">
    <br>
    <img alt="GitHub Workflow Status" src="https://img.shields.io/github/workflow/status/Jaysmito101/jmnet/jmnet%20Build?style=for-the-badge">
    <img alt="Maintenance" src="https://img.shields.io/maintenance/yes/2021?style=for-the-badge">
    <a href="https://patreon.com/jaysmito101"><img src="https://img.shields.io/endpoint.svg?url=https%3A%2F%2Fshieldsio-patreon.vercel.app%2Fapi%3Fusername%3Djaysmito101%26type%3Dpledges&style=for-the-badge" alt="Support me on Patreon" /></a>
</p>
<br/>


# jmnet


<a href="https://github.com/Jaysmito101/dynamic-badges/"><img src="https://dynamic-badges.maxalpha.repl.co/star?user=Jaysmito101&repo=jmnet&show=true"><img src="https://dynamic-badges.maxalpha.repl.co/star?image=true"></a>

# Docs : 
### For Help join our discord server : https://discord.gg/3h5S2cSbm8
### https://github.com/Jaysmito101/jmnet/wiki

# Getting Started

Download `jmnet.hpp`

Include it in your cpp file : 

    #define JMNET_IMPLEMENTATION
    #include "jmnet.hpp"

Then you need to initialize jmnet using : 

    jmnet::Initiaize();

Lastly after everything is done you need to do clean up.

    jmnet::Shutdown();

### Connecting to a server

    jmnet::TCPClient client; // Create the client
    client.Connect(jmnet::Address("example.com", 80));
    char data[4096];
    client.Receive(data, sizeof(data));
    std::cout << "Data : \n" << data << std::endl;

### Create a Server

    net::TCPServer server(80);
    server.Listen();
    jmnet::Socket* client = server.Accept();
    std::string data = "Hello!";
    client->Send(data);


# Support


Any support would be highly appretiated!

For example you could add a feature and contribute via pull requests or you could even report any issues with the program!

And the best thing you could do to support this project is spread word about this so that more people who might be interested in this may use this!

Please considering tweeting about this! 


<a href="https://dynamic-badges.maxalpha.repl.co/star?user=Jaysmito101&repo=jmnet&id=1"><img src="https://dynamic-badges.maxalpha.repl.co/star?image=true"></a>
<a href="https://dynamic-badges.maxalpha.repl.co/star?user=Jaysmito101&repo=jmnet&id=2"><img src="https://dynamic-badges.maxalpha.repl.co/star?image=true"></a>
<a href="https://dynamic-badges.maxalpha.repl.co/star?user=Jaysmito101&repo=jmnet&id=3"><img src="https://dynamic-badges.maxalpha.repl.co/star?image=true"></a>
<a href="https://dynamic-badges.maxalpha.repl.co/star?user=Jaysmito101&repo=jmnet&id=4"><img src="https://dynamic-badges.maxalpha.repl.co/star?image=true"></a>
<a href="https://dynamic-badges.maxalpha.repl.co/star?user=Jaysmito101&repo=jmnet&id=5"><img src="https://dynamic-badges.maxalpha.repl.co/star?image=true"></a>


![Views](https://dynamic-badges.maxalpha.repl.co/views?id=Jaysmito101.jmnet&style=for-the-badge&color=blue)
