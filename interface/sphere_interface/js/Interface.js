var ros = new ROSLIB.Ros({
  url : 'ws://localhost:9090'
});
ros.on('connection', function(){
  var fbDiv = document.getElementById('feedback');
  //fbDiv.innerHTML += "<p>Connected to websocket server.</p>";
});
ros.on('error', function(error){
  var fbDiv = document.getElementById('feedback');
  fbDiv.innerHTML += "<p>Error connecting to websocket server.</p>";
});
ros.on('close', function(){
  var fbDiv = document.getElementById('feedback');
  fbDiv.innerHTML += "<p>Connection to websocket server closed.</p>";
});

var interface = new ROSLIB.Topic({
  ros : ros,
  name : '/package/InterfaceSend2FPGA',
  messageType : 'motionpackage/InterfaceSend2FPGA'
});
var SendPackage = new ROSLIB.Message({
  Package : 0
});

var SectorPackage = new ROSLIB.Topic({
  ros : ros,
  name : '/package/Sector',
  messageType : 'std_msgs/Int16'
});
var SendSectorPackage = new ROSLIB.Message({
  data : 0
});
var SendPackageCallBack = new ROSLIB.Topic({
  ros : ros,
  name : '/package/motioncallback',
  messageType : 'std_msgs/Bool'
});
var InterfaceSaveData = new ROSLIB.Topic({
  ros : ros,
  name : '/package/InterfaceSaveData',
  messageType : 'motionpackage/InterfaceSaveData'
});
var SaveData = new ROSLIB.Message({
  data : 0,
  dataclass : 0,
  flag : 0
});

var InterfaceReadData = new ROSLIB.Topic({
  ros : ros,
  name : '/package/InterfaceReadSaveData',
  messageType : 'motionpackage/InterfaceSaveData'
});
var ReadData = new ROSLIB.Topic({
  ros : ros,
  name : '/package/InterfaceReadData',
  messageType : 'motionpackage/InterfaceSaveData'
});
var opendata = [];
var sum = 0;
SendPackageCallBack.subscribe(function(msg){
	if(msg.data == true)
	{
		document.getElementById('SendButton').disabled = false;
	}
	else if(msg.data == false)
	{
		alert("SendPackage fail");
	}
});
ReadData.subscribe(function(msg){
  opendata[sum++] = msg.data;
  if(msg.data == 0x23 && msg.dataclass == 2 && msg.flag == 1)
  {
    NewPosition();
    NewSpeed();
  }
  else if(msg.data == 0x23 && msg.dataclass == 3 && msg.flag == 1) 
  {
    NewMotion();
  }
  else if(msg.data == 0x2A && msg.dataclass == 2 && msg.flag == 0)
  {
    var num=document.getElementById('PositionTable').getElementsByTagName('div').length;
    document.getElementById('P_' + num).getElementsByClassName('textbox')[0].value = opendata[1];
    document.getElementById('S_' + num).getElementsByClassName('textbox')[0].value = opendata[1];
    for (var i = 0; i < 21; i++) 
    {
      document.getElementById('P_' + num).getElementsByClassName('textbox')[i+2].value = opendata[2*i + 2];
      document.getElementById('S_' + num).getElementsByClassName('textbox')[i+2].value = opendata[2*(i+1) + 1];
    }
    opendata = [];
    sum = 0;
  }
  else if(msg.data == 0x2A && msg.dataclass == 3 && msg.flag == 0)
  {
    var num=document.getElementById('MotionTable').getElementsByTagName('div').length;
    document.getElementById('M_' + num).getElementsByClassName('textbox')[0].value = opendata[1];
    for (var i = 0; i < 40; i++) 
    {
      document.getElementById('M_' + num).getElementsByClassName('textbox')[i+2].value = opendata[i+2];
    }
    opendata = [];
    sum = 0;
  }

});

function Send()
{
  document.getElementById('SendButton').disabled = true;
  var ActionList = [];
  var ID = Number(document.getElementById('SendID').value);
  var Sector = Number(document.getElementById('Sector').value);
  var count = 0;
  var MotorSum = 0;
  var sum = 0;
  ActionList[count++] = 83;
  ActionList[count++] = 84;
  ActionList[count++] = 243;
  ActionList[count++] = Sector;
  ActionList[count++] = 1;
  ActionList[count++] = Sector + 1;
  ActionList[count++] = 30;
  if (Sector == 29)
  {
    alert("Sector 29 is Locked. Please try again. ");
  }
  else if (Sector > 50)
  {
    alert("Sector is not find. Please try again. ");
  }
  else if (Sector != 29 && Sector <= 50)
  {
    for (var i = 1; i <= document.getElementById('PositionTable').getElementsByTagName('div').length; i++) 
    {
      if (ID == document.getElementById('P_' + i).getElementsByClassName('textbox')[0].value) 
      {
        for (var j = 1; j <= 21; j++) 
        {
          if (Number(document.getElementById('P_' + i).getElementsByClassName('textbox')[j + 1].value)) 
          {
            MotorSum++;
          }
        }
        ActionList[count++] = MotorSum;
        for (var j = 1; j <= 21; j++) 
        {
          if (Number(document.getElementById('P_' + i).getElementsByClassName('textbox')[j + 1].value)) 
          {
            ActionList[count++] = j;
            if (Number(document.getElementById('P_' + i).getElementsByClassName('textbox')[j + 1].value) >= 0)
            {
              ActionList[count++] = (Number(document.getElementById('P_' + i).getElementsByClassName('textbox')[j + 1].value)) & 0xff;
              ActionList[count++] = (((Number(document.getElementById('P_' + i).getElementsByClassName('textbox')[j + 1].value))>>8) & 0xff );
            }
            else if (Number(document.getElementById('P_' + i).getElementsByClassName('textbox')[j + 1].value) < 0)
            {
              var x = ~(Number(document.getElementById('P_' + i).getElementsByClassName('textbox')[j + 1].value)) + 1;
              ActionList[count++] = x & 0xff;
              ActionList[count++] = ((x>>8) & 0xff) | 0x80 ;
            }
            ActionList[count++] = (Number(document.getElementById('S_' + i).getElementsByClassName('textbox')[j + 1].value)) & 0xff;
            ActionList[count++] = ((Number(document.getElementById('S_' + i).getElementsByClassName('textbox')[j + 1].value)) >> 8) & 0xff;
            sum += ActionList[count-1] + ActionList[count-2] + ActionList[count-3] + ActionList[count-4] + ActionList[count-5];
          }
        }
        ActionList[count++] = 0;
        ActionList[count++] = 0;
        ActionList[count++] = sum%256;
        sum = 0;
        ActionList[count++] = 78;
        ActionList[count] = 69;
        MotorSum = 0;
        for (var a = 0; a < ActionList.length; a++) 
        {
          SendPackage.Package = ActionList[a];
          interface.publish(SendPackage);
          //alert(SendPackage.Package);
        }
      }
    }

    for (var i = 1; i <= document.getElementById('MotionTable').getElementsByTagName('div').length; i++) 
    {
      if (ID == document.getElementById('M_' + i).getElementsByClassName('textbox')[0].value) 
      {
        for (var j = 1; j <= 20; j++) 
        {
          if (Number(document.getElementById('M_' + i).getElementsByClassName('textbox')[j*2].value)) 
          {
            for (var l = 1; l <= document.getElementById('PositionTable').getElementsByTagName('div').length; l++) 
            {
              if (Number(document.getElementById('M_' + i).getElementsByClassName('textbox')[j*2].value) == Number(document.getElementById('P_' + l).getElementsByClassName('textbox')[0].value)) 
              {
                for (var k = 1; k <= 21; k++) 
                {
                  if (Number(document.getElementById('P_' + l).getElementsByClassName('textbox')[k + 1].value)) 
                  {
                    MotorSum++;
                  }
                }
                ActionList[count++] = MotorSum;
                for (var k = 1; k <= 21; k++) 
                {
                  if (Number(document.getElementById('P_' + l).getElementsByClassName('textbox')[k + 1].value)) 
                  {
                    ActionList[count++] = k;
                    if (Number(document.getElementById('P_' + l).getElementsByClassName('textbox')[k + 1].value) >= 0)
                    {
                      ActionList[count++] = (Number(document.getElementById('P_' + l).getElementsByClassName('textbox')[k + 1].value)) & 0xff;
                      ActionList[count++] = (Number(document.getElementById('P_' + l).getElementsByClassName('textbox')[k + 1].value) >> 8) & 0xff;
                    }
                    else if (Number(document.getElementById('P_' + l).getElementsByClassName('textbox')[k + 1].value) < 0)
                    {
                      ActionList[count++] = ~(Number(document.getElementById('P_' + l).getElementsByClassName('textbox')[k + 1].value) - 1) & 0xff;
                      ActionList[count++] = (~((Number(document.getElementById('P_' + l).getElementsByClassName('textbox')[k + 1].value) - 1 ) >>8) & 0xff) | 0x80;
                    }
                  
                    ActionList[count++] = (Number(document.getElementById('S_' + l).getElementsByClassName('textbox')[k + 1].value)) & 0xff;
                    ActionList[count++] = (Number(document.getElementById('S_' + l).getElementsByClassName('textbox')[k + 1].value) >> 8) & 0xff;
                    sum += ActionList[count-1] + ActionList[count-2] + ActionList[count-3] + ActionList[count-4] + ActionList[count-5];
                  }
                }
                ActionList[count++] = (Number(document.getElementById('M_' + i).getElementsByClassName('textbox')[j*2 + 1].value)) & 0xff;
                ActionList[count++] = (Number(document.getElementById('M_' + i).getElementsByClassName('textbox')[j*2 + 1].value) >> 8) & 0xff;
                sum += (Number(document.getElementById('M_' + i).getElementsByClassName('textbox')[j*2 + 1].value) & 0xff) + (Number(document.getElementById('M_' + i).getElementsByClassName('textbox')[j*2 + 1].value) >> 8)&0xff;
                ActionList[count++] = sum & 0xff;
                sum = 0;
                ActionList[count++] = 82;
                MotorSum = 0;
              }
            }

          }
        }
        ActionList[count - 1] = 78;
        ActionList[count] = 69;
        for (var a = 0; a < ActionList.length; a++) 
        {
          SendPackage.Package = ActionList[a];
          interface.publish(SendPackage);
          //alert(SendPackage.Package);
        }
        break;
      }
    }
  }
}
function execute()
{
  document.getElementById('executeButton').disabled = true;

  SendSectorPackage.data = Number(document.getElementById('Sector').value);
  SectorPackage.publish(SendSectorPackage);

  document.getElementById('executeButton').disabled = false;
}

function stand()
{
  document.getElementById('standButton').disabled = true;

  SendSectorPackage.data = 29;
  SectorPackage.publish(SendSectorPackage);
  
  document.getElementById('standButton').disabled = false;
}

function Save()
{
  for(var i = 0;i < document.getElementById('MotionTable').getElementsByTagName('div').length;i++)
  {
    SaveData.data = 0x23;
    SaveData.dataclass = 3;
    SaveData.flag = 1;
    InterfaceSaveData.publish(SaveData);

    SaveData.flag = 0;
    SaveData.data = Number(document.getElementById('MotionTable').getElementsByTagName('div')[i].getElementsByClassName('textbox')[0].value);

    InterfaceSaveData.publish(SaveData);
    SaveData.data = 3;
    InterfaceSaveData.publish(SaveData);

    for (var j = 2; j < document.getElementById('MotionTable').getElementsByTagName('div')[i].getElementsByClassName('textbox').length; j++) 
    {
      SaveData.data = Number(document.getElementById('MotionTable').getElementsByTagName('div')[i].getElementsByClassName('textbox')[j].value);
      InterfaceSaveData.publish(SaveData);
    }
    SaveData.data = 0x2A;
    InterfaceSaveData.publish(SaveData);
  }
  for(var i = 0;i < document.getElementById('PositionTable').getElementsByTagName('div').length;i++)
  {
    SaveData.data = 0x23;
    SaveData.dataclass = 2;
    SaveData.flag = 1;
    InterfaceSaveData.publish(SaveData);

    SaveData.flag = 0;
    SaveData.data = Number(document.getElementById('PositionTable').getElementsByTagName('div')[i].getElementsByClassName('textbox')[0].value);
    InterfaceSaveData.publish(SaveData);

    SaveData.data = 2;
    InterfaceSaveData.publish(SaveData);

    for (var j = 2; j < document.getElementById('PositionTable').getElementsByTagName('div')[i].getElementsByClassName('textbox').length; j++) 
    {
      SaveData.data = Number(document.getElementById('PositionTable').getElementsByTagName('div')[i].getElementsByClassName('textbox')[j].value);
      InterfaceSaveData.publish(SaveData);
      SaveData.data = Number(document.getElementById('SpeedTable').getElementsByTagName('div')[i].getElementsByClassName('textbox')[j].value);
      InterfaceSaveData.publish(SaveData);
      SaveData.data = 0;
      InterfaceSaveData.publish(SaveData);
    }
    SaveData.data = 0x2A;
    InterfaceSaveData.publish(SaveData);
  }
  SaveData.data = 0;
  SaveData.dataclass = 0;
  SaveData.flag = 1;
  InterfaceSaveData.publish(SaveData);
  SaveData.flag = 0;
}

function Read()
{
  InterfaceReadData.publish(SaveData);
}
