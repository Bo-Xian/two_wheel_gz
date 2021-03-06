var ros = new ROSLIB.Ros({
  url : 'ws://localhost:9090'
});
ros.on('connection', function(){
  var fbDiv = document.getElementById('feedback');
  console.log('Connection made!');
  document.getElementById('connected').style.display = 'inline';
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
  name : '/package/InterfaceSend2Sector',
  messageType: 'tku_msgs/InterfaceSend2Sector'
});
var SendPackage = new ROSLIB.Message({
  Package : 0,
  sectorname : ""
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
  messageType: 'std_msgs/Bool'
});
var ExecuteCallBack = new ROSLIB.Topic({
  ros: ros,
  name: '/package/executecallback',
  messageType: 'std_msgs/Bool'
});
var InterfaceSaveMotionData = new ROSLIB.Topic({
  ros : ros,
  name : '/package/InterfaceSaveMotion',
  messageType: 'tku_msgs/SaveMotion'
});
var SaveMotionData = new ROSLIB.Message({
    name : "",
    motionstate : 0,
    ID : 0,
    savestate : 0,
    saveflag : false,
    MotionList:[0],
    MotorData:[0]
});

var FirstSend = true;
SendPackageCallBack.subscribe(function(msg)
{
  if(msg.data == true)
  {
    document.getElementById('label').innerHTML = "Send sector is successful!!";
    document.getElementById('SendButton').disabled = false;
    document.getElementById('executeButton').disabled = false;
    document.getElementById('SaveButton').disabled = false;
    document.getElementById('ReadButton').disabled = false;
    document.getElementById('standButton').disabled = false;
    document.getElementById('CheckSum').disabled = false;
  }
  else if(msg.data == false)
  {
    document.getElementById('label').innerHTML = "Send sector is fail!! Please try again!!";
    document.getElementById('SendButton').disabled = false;
    document.getElementById('executeButton').disabled = false;
    document.getElementById('SaveButton').disabled = false;
    document.getElementById('ReadButton').disabled = false;
    document.getElementById('standButton').disabled = false;
    document.getElementById('CheckSum').disabled = false;
  }
});

ExecuteCallBack.subscribe(function (msg) {
  if (msg.data == true) {
    document.getElementById('label').innerHTML = "Execute is finish!!";
    document.getElementById('SendButton').disabled = false;
    document.getElementById('executeButton').disabled = false;
    document.getElementById('SaveButton').disabled = false;
    document.getElementById('ReadButton').disabled = false;
    document.getElementById('standButton').disabled = false;
    document.getElementById('CheckSum').disabled = false;
  }
});

function sleep(ms)
{
  var starttime = new Date().getTime();
  do{

  }while((new Date().getTime() - starttime) < ms)
}
function Locked()
{
  if (!document.getElementById('Locked29').checked)
  {
    document.getElementById('label').innerHTML = "Sector 29 is Unlocked";
  }
  else if (document.getElementById('Locked29').checked)
  {
    document.getElementById('label').innerHTML = "Sector 29 is Locked";
  }
}

function Send()
{
  document.getElementById('label').innerHTML = "";
  var MotionList = [];
  var ID = Number(document.getElementById('SendID').value);
  var Sector = Number(document.getElementById('Sector').value);
  var count = 0;
  var MotorSum = 0;
  var sum = 0;
  var checksum = 0;
  var checksum_Lhand = 0;
  var checksum_Rhand = 0;
  var checksum_Lfoot = 0;
  var checksum_Rfoot = 0;
  SendPackage.sectorname = document.getElementById('Sector').value;
  MotionList[count++] = 83;
  MotionList[count++] = 84;
  if (document.getElementById('Locked29').checked && Number(document.getElementById('Sector').value) == 29)
  {
    alert("Sector 29 is Locked. Please try again. ");
  }
  else if (Sector < 1)
  {
    alert("Sector is not find. Please try again. ");
  }
  else
  {
    for (var i = 0; i < document.getElementById('AbsolutePositionTable').getElementsByTagName('div').length; i++) 
    {
      if (ID == document.getElementById('AbsolutePositionTable').getElementsByTagName('div')[i].getElementsByClassName('textbox')[0].value) 
      {
        MotorSum = 21;
        MotionList[count++] = 242;
        for (var j = 0; j < 21; j++) 
        {
          MotionList[count] = (Number(document.getElementById('AbsoluteSpeedTable').getElementsByTagName('div')[i + 1].getElementsByClassName('textbox')[j + 1].value)) & 0xff;
          checksum += MotionList[count];
          count++;
          MotionList[count] = (((Number(document.getElementById('AbsoluteSpeedTable').getElementsByTagName('div')[i + 1].getElementsByClassName('textbox')[j + 1].value)) >> 8) & 0xff);
          checksum += MotionList[count];
          count++;
          MotionList[count] = (Number(document.getElementById('AbsolutePositionTable').getElementsByTagName('div')[i + 1].getElementsByClassName('textbox')[j + 1].value)) & 0xff;
          checksum += MotionList[count];
          count++;
          MotionList[count] = (((Number(document.getElementById('AbsolutePositionTable').getElementsByTagName('div')[i + 1].getElementsByClassName('textbox')[j + 1].value)) >> 8) & 0xff);
          checksum += MotionList[count];
          count++;
          if(j < 4)
          {
            checksum_Lhand = checksum;
          }
          else if(j < 8)
          {
            checksum_Rhand = checksum - checksum_Lhand;
          }
          else if(j < 15)
          {
            checksum_Lfoot = checksum - checksum_Lhand - checksum_Rhand;
          }
          else
          {
            checksum_Rfoot = checksum - checksum_Lhand - checksum_Rhand - checksum_Lfoot;
          }
        }
        MotionList[count++] = checksum_Lhand & 0xff;
        MotionList[count++] = checksum_Rhand & 0xff;
        MotionList[count++] = checksum_Lfoot & 0xff;
        MotionList[count++] = checksum_Rfoot & 0xff;
        MotionList[count++] = count - 7;
        MotionList[count++] = 78;
        MotionList[count] = 69;
        for (var a = 0; a < MotionList.length; a++) 
        {
          SendPackage.Package = MotionList[a];
          interface.publish(SendPackage);
          console.log(SendPackage.Package);
          sleep(2);
        }
        if (FirstSend) 
        {
          for (var a = 0; a < MotionList.length; a++) 
          {
            SendPackage.Package = MotionList[a];
            interface.publish(SendPackage);
            console.log(SendPackage.Package);
            sleep(2);
          }
          FirstSend = false;
        }
        break;
      }
      
    }
    for (var i = 0; i < document.getElementById('RelativePositionTable').getElementsByTagName('div').length; i++) 
    {
      if (ID == document.getElementById('RelativePositionTable').getElementsByTagName('div')[i].getElementsByClassName('textbox')[0].value) 
      {
        MotorSum = 21;
        MotionList[count++] = 243;
        for (var j = 0; j < 21; j++) 
        {
          MotionList[count] = (Number(document.getElementById('RelativeSpeedTable').getElementsByTagName('div')[i + 1].getElementsByClassName('textbox')[j + 1].value)) & 0xff;
          checksum += MotionList[count];
          count++;
          MotionList[count] = (((Number(document.getElementById('RelativeSpeedTable').getElementsByTagName('div')[i + 1].getElementsByClassName('textbox')[j + 1].value)) >> 8) & 0xff);
          checksum += MotionList[count];
          count++;
          if (Number(document.getElementById('RelativePositionTable').getElementsByTagName('div')[i + 1].getElementsByClassName('textbox')[j + 1].value) >= 0) 
          {
            MotionList[count] = (Number(document.getElementById('RelativePositionTable').getElementsByTagName('div')[i + 1].getElementsByClassName('textbox')[j + 1].value)) & 0xff;
            checksum += MotionList[count];
            count++;
            MotionList[count] = (((Number(document.getElementById('RelativePositionTable').getElementsByTagName('div')[i + 1].getElementsByClassName('textbox')[j + 1].value)) >> 8) & 0xff);
            checksum += MotionList[count];
            count++;
          }
          else if (Number(document.getElementById('RelativePositionTable').getElementsByTagName('div')[i + 1].getElementsByClassName('textbox')[j + 1].value) < 0) 
          {
            var x = ~(Number(document.getElementById('RelativePositionTable').getElementsByTagName('div')[i + 1].getElementsByClassName('textbox')[j + 1].value)) + 1;
            MotionList[count] = x & 0xff;
            checksum += MotionList[count];
            count++;
            MotionList[count] = ((x >> 8) & 0xff) | 0x80;
            checksum += MotionList[count];
            count++;
          }
          //MotionList[count++] = Number(document.getElementById('RelativePositionTable').getElementsByTagName('div')[i+1].getElementsByClassName('textbox')[j + 1].value);
          if (j < 4) 
          {
            checksum_Lhand = checksum;
          }
          else if (j < 8) 
          {
            checksum_Rhand = checksum - checksum_Lhand;
          }
          else if (j < 15) 
          {
            checksum_Lfoot = checksum - checksum_Lhand - checksum_Rhand;
          }
          else 
          {
            checksum_Rfoot = checksum - checksum_Lhand - checksum_Rhand - checksum_Lfoot;
          }
        }
        MotionList[count++] = checksum_Lhand & 0xff;
        MotionList[count++] = checksum_Rhand & 0xff;
        MotionList[count++] = checksum_Lfoot & 0xff;
        MotionList[count++] = checksum_Rfoot & 0xff;
        MotionList[count++] = count - 7;
        MotionList[count++] = 78;
        MotionList[count] = 69;
        for (var a = 0; a < MotionList.length; a++) 
        {
          SendPackage.Package = MotionList[a];
          interface.publish(SendPackage);
          console.log(SendPackage.Package);
          sleep(2);
        }
        if (FirstSend) 
        {
          for (var a = 0; a < MotionList.length; a++) 
          {
            SendPackage.Package = MotionList[a];
            interface.publish(SendPackage);
            console.log(SendPackage.Package);
            sleep(2);
          }
          FirstSend = false;
        }
        break;
      }
      
    }

    for (var i = 0; i < document.getElementById('MotionTable').getElementsByTagName('div').length; i++) 
    {
      if (ID == document.getElementById('MotionTable').getElementsByTagName('div')[i].getElementsByClassName('textbox')[0].value) 
      {
        MotorSum = 21;
        MotionList[count++] = 244;
        for (var j = 1; j <= 20; j++) 
        {
          if (Number(document.getElementById('MotionTable').getElementsByTagName('div')[i+1].getElementsByClassName('textbox')[j*2 - 1].value)) 
          {
            for (var l = 0; l < document.getElementById('RelativePositionTable').getElementsByTagName('div').length; l++) 
            {
              if (Number(document.getElementById('MotionTable').getElementsByTagName('div')[i+1].getElementsByClassName('textbox')[j*2 -1].value) == Number(document.getElementById('RelativePositionTable').getElementsByTagName('div')[l].getElementsByClassName('textbox')[0].value)) 
              {
                for (var k = 0; k < 21; k++) 
                {
                  MotionList[count++] = (Number(document.getElementById('RelativeSpeedTable').getElementsByTagName('div')[l + 1].getElementsByClassName('textbox')[k + 1].value)) & 0xff;
                  MotionList[count++] = (((Number(document.getElementById('RelativeSpeedTable').getElementsByTagName('div')[l + 1].getElementsByClassName('textbox')[k + 1].value)) >> 8) & 0xff);
                  if (Number(document.getElementById('RelativePositionTable').getElementsByTagName('div')[l + 1].getElementsByClassName('textbox')[k + 1].value) >= 0) 
                  {
                    MotionList[count++] = (Number(document.getElementById('RelativePositionTable').getElementsByTagName('div')[l + 1].getElementsByClassName('textbox')[k + 1].value)) & 0xff;
                    MotionList[count++] = (((Number(document.getElementById('RelativePositionTable').getElementsByTagName('div')[l + 1].getElementsByClassName('textbox')[k + 1].value)) >> 8) & 0xff);
                  }
                  else if (Number(document.getElementById('RelativePositionTable').getElementsByTagName('div')[l + 1].getElementsByClassName('textbox')[k + 1].value) < 0) 
                  {
                    var x = ~(Number(document.getElementById('RelativePositionTable').getElementsByTagName('div')[l + 1].getElementsByClassName('textbox')[k + 1].value)) + 1;
                    MotionList[count++] = x & 0xff;
                    MotionList[count++] = ((x >> 8) & 0xff) | 0x80;
                  }
                  //MotionList[count++] = Number(document.getElementById('RelativePositionTable').getElementsByTagName('div')[i+1].getElementsByClassName('textbox')[j + 1].value);
                }
                MotionList[count++] = 68;
                MotionList[count++] = 89;
                MotionList[count++] = Number(document.getElementById('MotionTable').getElementsByTagName('div')[i + 1].getElementsByClassName('textbox')[j * 2].value);
                break;
              }
            }
          }
        }
        MotionList[count++] = 69; //finish big motion
        MotionList[count++] = 78; //finish big motion
        MotionList[count++] = count - 3;
        MotionList[count++] = 78;
        MotionList[count] = 69;
        for (var a = 0; a < MotionList.length; a++) 
        {
          SendPackage.Package = MotionList[a];
          interface.publish(SendPackage);
          console.log(SendPackage.Package);
          sleep(2);
        }
        if (FirstSend) 
        {
          for (var a = 0; a < MotionList.length; a++) 
          {
            SendPackage.Package = MotionList[a];
            interface.publish(SendPackage);
            console.log(SendPackage.Package);
            sleep(2);
          }
          FirstSend = false;
        }
        break;
      }
    }
  }
  MotionList.length = 0;
}
function execute()
{
  document.getElementById('SendButton').disabled = true;
  document.getElementById('executeButton').disabled = true;
  document.getElementById('SaveButton').disabled = true;
  document.getElementById('ReadButton').disabled = true;
  document.getElementById('standButton').disabled = true;
  document.getElementById('CheckSum').disabled = true;

  SendSectorPackage.data = Number(document.getElementById('Sector').value);
  SectorPackage.publish(SendSectorPackage);
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
  SaveMotionData.savestate = 0;
  SaveMotionData.name = document.getElementById('filename').value;
  for(var i = 0;i < document.getElementById('MotionTable').getElementsByTagName('div').length;i+=2)
  {
    SaveMotionData.motionstate = 0;
    SaveMotionData.ID = Number(document.getElementById('MotionTable').getElementsByTagName('div')[i].getElementsByClassName('textbox')[0].value);
    for(var j = 0; j < 40; j++)
    {
      SaveMotionData.MotionList[j] = Number(document.getElementById('MotionTable').getElementsByTagName('div')[i+1].getElementsByClassName('textbox')[j+1].value);
    }
    InterfaceSaveMotionData.publish(SaveMotionData);
  }

  for(var i = 0;i < document.getElementById('RelativePositionTable').getElementsByTagName('div').length;i+=2)
  {
    SaveMotionData.motionstate = 1;
    SaveMotionData.ID = Number(document.getElementById('RelativePositionTable').getElementsByTagName('div')[i].getElementsByClassName('textbox')[0].value);
    for(var j = 0; j < 21; j++)
    {
      SaveMotionData.MotorData[j] = Number(document.getElementById('RelativePositionTable').getElementsByTagName('div')[i+1].getElementsByClassName('textbox')[j+1].value);
    }

    InterfaceSaveMotionData.publish(SaveMotionData);
    SaveMotionData.motionstate = 2;
    SaveMotionData.ID = Number(document.getElementById('RelativeSpeedTable').getElementsByTagName('div')[i].getElementsByClassName('textbox')[0].value);
    for(var j = 0; j < 21; j++)
    {
      SaveMotionData.MotorData[j] = Number(document.getElementById('RelativeSpeedTable').getElementsByTagName('div')[i+1].getElementsByClassName('textbox')[j+1].value);
    }
    InterfaceSaveMotionData.publish(SaveMotionData);
  }
  
  for(var i = 0;i < document.getElementById('AbsolutePositionTable').getElementsByTagName('div').length;i+=2)
  {
    SaveMotionData.motionstate = 3;
    SaveMotionData.ID = Number(document.getElementById('AbsolutePositionTable').getElementsByTagName('div')[i].getElementsByClassName('textbox')[0].value);
    for(var j = 0; j < 21; j++)
    {
      SaveMotionData.MotorData[j] = Number(document.getElementById('AbsolutePositionTable').getElementsByTagName('div')[i+1].getElementsByClassName('textbox')[j+1].value);
    }
    InterfaceSaveMotionData.publish(SaveMotionData);
    SaveMotionData.motionstate = 4;
    SaveMotionData.ID = Number(document.getElementById('AbsoluteSpeedTable').getElementsByTagName('div')[i].getElementsByClassName('textbox')[0].value);
    for(var j = 0; j < 21; j++)
    {
      SaveMotionData.MotorData[j] = Number(document.getElementById('AbsoluteSpeedTable').getElementsByTagName('div')[i+1].getElementsByClassName('textbox')[j+1].value);
    }
    InterfaceSaveMotionData.publish(SaveMotionData);
  }
  SaveMotionData.saveflag = true;
  InterfaceSaveMotionData.publish(SaveMotionData);
  SaveMotionData.saveflag = false;
}

function SaveStand()
{
  SaveMotionData.savestate = 1;
  SaveMotionData.name = document.getElementById('filename').value;
  for(var i = 0;i < document.getElementById('MotionTable').getElementsByTagName('div').length;i+=2)
  {
    SaveMotionData.motionstate = 0;
    SaveMotionData.ID = Number(document.getElementById('MotionTable').getElementsByTagName('div')[i].getElementsByClassName('textbox')[0].value);
    for(var j = 0; j < 40; j++)
    {
      SaveMotionData.MotionList[j] = Number(document.getElementById('MotionTable').getElementsByTagName('div')[i+1].getElementsByClassName('textbox')[j+1].value);
    }
    InterfaceSaveMotionData.publish(SaveMotionData);
  }
  
  for(var i = 0;i < document.getElementById('RelativePositionTable').getElementsByTagName('div').length;i+=2)
  {
    SaveMotionData.motionstate = 1;
    SaveMotionData.ID = Number(document.getElementById('RelativePositionTable').getElementsByTagName('div')[i].getElementsByClassName('textbox')[0].value);
    for(var j = 0; j < 21; j++)
    {
      SaveMotionData.MotorData[j] = Number(document.getElementById('RelativePositionTable').getElementsByTagName('div')[i+1].getElementsByClassName('textbox')[j+1].value);
    }
    InterfaceSaveMotionData.publish(SaveMotionData);
    SaveMotionData.motionstate = 2;
    SaveMotionData.ID = Number(document.getElementById('RelativeSpeedTable').getElementsByTagName('div')[i].getElementsByClassName('textbox')[0].value);
    for(var j = 0; j < 21; j++)
    {
      SaveMotionData.MotorData[j] = Number(document.getElementById('RelativeSpeedTable').getElementsByTagName('div')[i+1].getElementsByClassName('textbox')[j+1].value);
    }
    InterfaceSaveMotionData.publish(SaveMotionData);
  }
  
  
  for(var i = 0;i < document.getElementById('AbsolutePositionTable').getElementsByTagName('div').length;i+=2)
  {
    SaveMotionData.motionstate = 3;
    SaveMotionData.ID = Number(document.getElementById('AbsolutePositionTable').getElementsByTagName('div')[i].getElementsByClassName('textbox')[0].value);
    for(var j = 0; j < 21; j++)
    {
      SaveMotionData.MotorData[j] = Number(document.getElementById('AbsolutePositionTable').getElementsByTagName('div')[i+1].getElementsByClassName('textbox')[j+1].value);
    }
    InterfaceSaveMotionData.publish(SaveMotionData);
    SaveMotionData.motionstate = 4;
    SaveMotionData.ID = Number(document.getElementById('AbsoluteSpeedTable').getElementsByTagName('div')[i].getElementsByClassName('textbox')[0].value);
    for(var j = 0; j < 21; j++)
    {
      SaveMotionData.MotorData[j] = Number(document.getElementById('AbsoluteSpeedTable').getElementsByTagName('div')[i+1].getElementsByClassName('textbox')[j+1].value);
    }
    InterfaceSaveMotionData.publish(SaveMotionData);
  }
  SaveMotionData.saveflag = true;
  InterfaceSaveMotionData.publish(SaveMotionData);
  SaveMotionData.saveflag = false;
}
function Read()
{
  var LoadParameterClient = new ROSLIB.Service({
    ros : ros,
    name : '/package/InterfaceReadSaveMotion',
    serviceType: 'tku_msgs/ReadMotion'
  });
  var parameter_request = new ROSLIB.ServiceRequest({
    read : true,
    name : document.getElementById('filename').value,
    readstate : 0
  });
  LoadParameterClient.callService(parameter_request , function(MotionData){
    var motionlistcnt = 0;
    var relativepositioncnt = 0;
    var relativespeedcnt = 0;
    var absolutepositioncnt = 0;
    var absolutespeedcnt = 0;
    for(var i = 0; i < MotionData.VectorCnt; i++)
    {
      //console.log(MotionData.VectorCnt);
      //console.log(MotionData.motionstate[i]);
      switch(MotionData.motionstate[i])
      {
        case 0:
          NewMotion();
          //console.log(MotionData.ID[i]);
          document.getElementById('MotionTable').getElementsByTagName('div')[motionlistcnt*2].getElementsByClassName('textbox')[0].value = MotionData.ID[i];
          for(var j = 0; j < 40; j++)
          {
            document.getElementById('MotionTable').getElementsByTagName('div')[motionlistcnt*2+1].getElementsByClassName('textbox')[j+1].value = MotionData.MotionList[motionlistcnt*40+j];
            
          }
          motionlistcnt++;
          break;
        case 1:
          NewRelativePosition();
          //console.log(MotionData.ID[i]);
          document.getElementById('RelativePositionTable').getElementsByTagName('div')[relativepositioncnt*2].getElementsByClassName('textbox')[0].value = MotionData.ID[i];
          for(var j = 0; j < 21; j++)
          {
            document.getElementById('RelativePositionTable').getElementsByTagName('div')[relativepositioncnt*2+1].getElementsByClassName('textbox')[j+1].value = MotionData.RelativeData[relativepositioncnt*21+relativespeedcnt*21+j];
          }
          relativepositioncnt++;
          break;
        case 2:
          NewRelativeSpeed();
          //console.log(MotionData.ID[i]);
          document.getElementById('RelativeSpeedTable').getElementsByTagName('div')[relativespeedcnt*2].getElementsByClassName('textbox')[0].value = MotionData.ID[i];
          for(var j = 0; j < 21; j++)
          {
            document.getElementById('RelativeSpeedTable').getElementsByTagName('div')[relativespeedcnt*2+1].getElementsByClassName('textbox')[j+1].value = MotionData.RelativeData[relativepositioncnt*21+relativespeedcnt*21+j];
          }
          relativespeedcnt++;
          break;
        case 3:
          NewAbsolutePosition();
          //console.log(MotionData.ID[i]);
          document.getElementById('AbsolutePositionTable').getElementsByTagName('div')[absolutepositioncnt*2].getElementsByClassName('textbox')[0].value = MotionData.ID[i];
          for(var j = 0; j < 21; j++)
          {
            document.getElementById('AbsolutePositionTable').getElementsByTagName('div')[absolutepositioncnt*2+1].getElementsByClassName('textbox')[j+1].value = MotionData.AbsoluteData[absolutepositioncnt*21+absolutespeedcnt*21+j];
          }
          absolutepositioncnt++;
          break;
        case 4:
          NewAbsoluteSpeed();
          //console.log(MotionData.ID[i]);
          document.getElementById('AbsoluteSpeedTable').getElementsByTagName('div')[absolutespeedcnt*2].getElementsByClassName('textbox')[0].value = MotionData.ID[i];
          for(var j = 0; j < 21; j++)
          {
            document.getElementById('AbsoluteSpeedTable').getElementsByTagName('div')[absolutespeedcnt*2+1].getElementsByClassName('textbox')[j+1].value = MotionData.AbsoluteData[absolutepositioncnt*21+absolutespeedcnt*21+j];
          }
          absolutespeedcnt++;
          break;
      }
    }
  });
}
function ReadStand()
{
  var LoadParameterClient = new ROSLIB.Service({
    ros : ros,
    name : '/package/InterfaceReadSaveMotion',
    serviceType: 'tku_msgs/ReadMotion'
  });
  var parameter_request = new ROSLIB.ServiceRequest({
    read : true,
    name : document.getElementById('filename').value,
    readstate : 1
  });
  LoadParameterClient.callService(parameter_request , function(MotionData){
    var motionlistcnt = 0;
    var relativepositioncnt = 0;
    var relativespeedcnt = 0;
    var absolutepositioncnt = 0;
    var absolutespeedcnt = 0;
    for(var i = 0; i < MotionData.VectorCnt; i++)
    {
      console.log(MotionData.motionstate[i]);
      switch(MotionData.motionstate[i])
      {
        case 0:
          NewMotion();
          console.log(MotionData.ID[i]);
          document.getElementById('MotionTable').getElementsByTagName('div')[motionlistcnt*2].getElementsByClassName('textbox')[0].value = MotionData.ID[i];
          for(var j = 0; j < 40; j++)
          {
            document.getElementById('MotionTable').getElementsByTagName('div')[motionlistcnt*2+1].getElementsByClassName('textbox')[j+1].value = MotionData.MotionList[motionlistcnt*40+j];
          }
          motionlistcnt++;
          break;
        case 1:
          NewRelativePosition();
          
          console.log(MotionData.ID[i]);
          document.getElementById('RelativePositionTable').getElementsByTagName('div')[relativepositioncnt*2].getElementsByClassName('textbox')[0].value = MotionData.ID[i];
          for(var j = 0; j < 21; j++)
          {
            document.getElementById('RelativePositionTable').getElementsByTagName('div')[relativepositioncnt*2+1].getElementsByClassName('textbox')[j+1].value = MotionData.RelativeData[relativepositioncnt*21+relativespeedcnt*21+j];
          }
          relativepositioncnt++;
          break;
        case 2:
          NewRelativeSpeed();
          
          console.log(MotionData.ID[i]);
          document.getElementById('RelativeSpeedTable').getElementsByTagName('div')[relativespeedcnt*2].getElementsByClassName('textbox')[0].value = MotionData.ID[i];
          for(var j = 0; j < 21; j++)
          {
            document.getElementById('RelativeSpeedTable').getElementsByTagName('div')[relativespeedcnt*2+1].getElementsByClassName('textbox')[j+1].value = MotionData.RelativeData[relativepositioncnt*21+relativespeedcnt*21+j];
          }
          relativespeedcnt++;
          break;
        case 3:
          NewAbsolutePosition();
          console.log(MotionData.ID[i]);
          document.getElementById('AbsolutePositionTable').getElementsByTagName('div')[absolutepositioncnt*2].getElementsByClassName('textbox')[0].value = MotionData.ID[i];
          for(var j = 0; j < 21; j++)
          {
            document.getElementById('AbsolutePositionTable').getElementsByTagName('div')[absolutepositioncnt*2+1].getElementsByClassName('textbox')[j+1].value = MotionData.AbsoluteData[absolutepositioncnt*21+absolutespeedcnt*21+j];
          }
          absolutepositioncnt++;
          break;
        case 4:
          NewAbsoluteSpeed();
          console.log(MotionData.ID[i]);
          document.getElementById('AbsoluteSpeedTable').getElementsByTagName('div')[absolutespeedcnt*2].getElementsByClassName('textbox')[0].value = MotionData.ID[i];
          for(var j = 0; j < 21; j++)
          {
            document.getElementById('AbsoluteSpeedTable').getElementsByTagName('div')[absolutespeedcnt*2+1].getElementsByClassName('textbox')[j+1].value = MotionData.AbsoluteData[absolutepositioncnt*21+absolutespeedcnt*21+j];
          }
          absolutespeedcnt++;
          break;
      }
    }
  });
}
function CheckSumButton()
{

    var ID = Number(document.getElementById('SendID').value);
    for (var i = 0; i < document.getElementById('AbsolutePositionTable').getElementsByTagName('div').length; i++) 
    {
        if (ID == document.getElementById('AbsolutePositionTable').getElementsByTagName('div')[i].getElementsByClassName('textbox')[0].value) 
        {
            
            MotorSum = 21;
            
            for (var j = 1; j < 22; j++) 
            {
                document.getElementById("CheckSumBox").getElementsByTagName("div")[j-1].innerHTML = "";
                document.getElementById("CheckSumBox").getElementsByTagName("div")[j-1].innerHTML += "M" + j + ": " + (Number(document.getElementById('AbsolutePositionTable').getElementsByTagName('div')[i].getElementsByClassName('textbox')[j + 1].value));
            }
            break;
        }
    }
    for (var i = 0; i < document.getElementById('RelativePositionTable').getElementsByTagName('div').length; i++) 
    {
        if (ID == document.getElementById('RelativePositionTable').getElementsByTagName('div')[i].getElementsByClassName('textbox')[0].value) 
        {
            MotorSum = 21;

            for (var j = 1; j < 22; j++) 
            {
                document.getElementById("CheckSumBox").getElementsByTagName("div")[j-1].innerHTML = "";
                document.getElementById("CheckSumBox").getElementsByTagName("div")[j-1].innerHTML += "M" + j + ": " + (Number(document.getElementById('RelativePositionTable').getElementsByTagName('div')[i].getElementsByClassName('textbox')[j + 1].value));
            }
            break;
        }
    }
    for (var i = 0 ; i < document.getElementById('MotionTable').getElementsByTagName('div').length; i++) 
    {
        if (ID == document.getElementById('MotionTable').getElementsByTagName('div')[i].getElementsByClassName('textbox')[0].value) 
        {
            var Sum = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
            for (var j = 1; j <= 20; j++) 
            {
                if (Number(document.getElementById('MotionTable').getElementsByTagName('div')[i].getElementsByClassName('textbox')[j*2].value)) 
                {
                    for (var k = 0; k < document.getElementById('RelativePositionTable').getElementsByTagName('div').length; k++) 
                    {
                        if (Number(document.getElementById('MotionTable').getElementsByTagName('div')[i].getElementsByClassName('textbox')[j*2].value) == Number(document.getElementById('RelativePositionTable').getElementsByTagName('div')[k].getElementsByClassName('textbox')[0].value)) 
                        {
                            MotorSum = 21;
                    
                            for (var l = 1; l <= 21; l++) 
                            {
                                
                                Sum[l-1] += (Number(document.getElementById('RelativePositionTable').getElementsByTagName('div')[k].getElementsByClassName('textbox')[l + 1].value));

                            }
                        }
                    }
                }
            }     

            for (var j = 0; j < MotorSum; j++) 
            {
                document.getElementById("CheckSumBox").getElementsByTagName("div")[j].innerHTML = "";
                document.getElementById("CheckSumBox").getElementsByTagName("div")[j].innerHTML += "M" + j + ": " + Sum[j];
            }
            break;
        }
    }
}
function resetfunction()
{
  document.getElementById('SendButton').disabled = false;
  document.getElementById('executeButton').disabled = false;
  document.getElementById('SaveButton').disabled = false;
  document.getElementById('ReadButton').disabled = false;
  document.getElementById('standButton').disabled = false;
  document.getElementById('CheckSum').disabled = false;
}
