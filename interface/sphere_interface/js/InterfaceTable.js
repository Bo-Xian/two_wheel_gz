function NewMotion(){
  var num=document.getElementById('MotionTable').getElementsByTagName('div').length+1;
  var div=document.createElement('div');

  div.id='M_'+num;
  for (var i = 1; i <= 2; i++) 
  {
    var input=document.createElement('input');
    input.type='text';
    input.id='M_' + num + '_' + i;
    input.className='textbox';
    input.value=-1;
    div.appendChild(input);
  }
  for (var i = 1; i <= 40 ; i++) 
  {     
    if (i%2==1) 
    {
      var input=document.createElement('input');
      input.type='text';
      input.id='Action_'+num+'_'+(i+1)/2;
      input.className='textbox';
      input.value=0;
      div.appendChild(input);
      
    }  
    if (i%2==0) 
    {
      var input=document.createElement('input');
      input.type='text';
      input.id='Delay_'+num+'_'+i/2;
      input.className='textbox';
      input.value=0;
      div.appendChild(input);
    }
  }
  document.getElementById('MotionTable').appendChild(div);
  for ( var i=1; i<=42 ;i++)
  {
    document.getElementById('M_' + num).getElementsByClassName('textbox')[i-1].onchange = function() {changemotion()};
  }
}
function DeleteMotion(){
  var num=document.getElementById('MotionTable').getElementsByTagName('div').length;
  document.getElementById('MotionTable').removeChild(document.getElementById('M_'+num));
}
function NewPosition(){
  var num=document.getElementById('PositionTable').getElementsByTagName('div').length+1;
  var div=document.createElement('div');
  for (var i = 1; i <= 2 ; i++)
  {
    div.id='P_'+num;
    var input=document.createElement('input');
    input.type='text';
    input.id='P_' + num + '_' +i;
    input.className='textbox';
    input.value=-1;
    div.appendChild(input);
  } 
  for (var i = 1; i <= 21 ; i++) 
  {
    var input=document.createElement('input');
    input.type='text';
    input.id='Position_'+num+'_'+i;
    input.className='textbox';
    input.value=0;
    div.appendChild(input);
  }  
  document.getElementById('PositionTable').appendChild(div);
  for ( var i = 1; i<=23; i++)
  {
    document.getElementById('P_' + num).getElementsByClassName('textbox')[i-1].onchange = function() {changeposition()};
  }
}
function NewSpeed()
{
  var num=document.getElementById('SpeedTable').getElementsByTagName('div').length+1;
  var div=document.createElement('div');  
  for (var i = 1; i <= 2; i++) 
  {
    div.id='S_'+num;
    var input=document.createElement('input');
    input.type='text';
    input.id='S_'+num+'_'+i;
    input.className='textbox';
    input.value=0;
    div.appendChild(input);
  }  
  for (var i = 1; i <= 21; i++) 
  {
    var input=document.createElement('input');
    input.type='text';
    input.id='Speed_'+num+'_'+i;
    input.className='textbox';
    input.value=511;
    div.appendChild(input);
  }
  document.getElementById('SpeedTable').appendChild(div);
}
// function NewAcceleration(){
//   var num=document.getElementById('AccelerationTable').getElementsByTagName('div').length+1;
//   var div=document.createElement('div');  
//     for (var i = 1; i <= 2; i++) 
//     {
//       div.id='A_'+num;
//       var input=document.createElement('input');
//       input.type='text';
//       input.id='A_'+num+'_'+i;
//       input.className='textbox';
//       input.value=0;
//       div.appendChild(input);
//       document.getElementById('AccelerationTable').appendChild(div);
//     }
//     for (var i = 1; i <= 21; i++) 
//     {
//       var input=document.createElement('input');
//       input.type='text';
//       input.id='Acceleration_'+num+'_'+i;
//       input.className='textbox';
//       input.value=0;
//       div.appendChild(input);
//       document.getElementById('AccelerationTable').appendChild(div);
//     }
// }
function New()
{
  NewPosition();
  NewSpeed();
  //NewAcceleration();
}
function DeleteNew()
{
  var num=document.getElementById('PositionTable').getElementsByTagName('div').length;
  document.getElementById('PositionTable').removeChild(document.getElementById('P_'+num));
  var num=document.getElementById('SpeedTable').getElementsByTagName('div').length;
  document.getElementById('SpeedTable').removeChild(document.getElementById('S_'+num));
  //var num=document.getElementById('AccelerationTable').getElementsByTagName('div').length;
  //document.getElementById('AccelerationTable').removeChild(document.getElementById('A_'+num));
}


function changemotion()
{
  // var num = document.getElementById('MotionTable').getElementsByTagName('div').length;
  // for (var i = 1; i <= num; i++) 
  // {
  //   MotionArray[i-1] = [];
  //   for (var j = 1; j <= 30; j++) 
  //   {
  //     MotionArray[i-1][j-1] = Number(document.getElementById('M_' + i).getElementsByClassName('textbox')[j-1].value);
  //   }
  // }
}
function changeposition()
{
  // var num = document.getElementById('PositionTable').getElementsByTagName('div').length;
  // for (var i = 1; i <= num; i++) 
  // {
  //   PositionArray[i-1] = [];
  //   for (var j = 1; j <= 23; j++) 
  //   {
  //     PositionArray[i-1][j-1] = Number(document.getElementById('P_' + i).getElementsByClassName('textbox')[j-1].value);
  //   }
  // }
}