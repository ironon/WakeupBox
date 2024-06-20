

<script>
  let alarmTime = '';
  let locked = true
  let hexKey = "7cb5d18ae409a37c10d3fdf3b68408ea3503d0335796bf949914e400ae2cc27d48ffa59034c01d033a6a854847561c5f2a1f19567e4f569635862d2550a6a1a50de09a20ee3b6b3946157353d568b3ee"
  let currentTime = new Date().toLocaleTimeString();
  let lockedIn = "0"

  let beepLength = 1000
  let beepCount = 10
  let beepDelay = 1000
  const updateLocks = async () => {
    // send beepLength, beepCount, and beepDelay to server
    await fetch(`/setLocks?beepLength=${beepLength}&beepCount=${beepCount}&beepDelay=${beepDelay}?key=${hexKey}`)
  }
  const getSavedTime = async () => {  
    // fetch mm/ss time from server, which is just /getTime
    const response = await fetch(`/getTime?key=${hexKey}`);
    let a = await response.text();
    console.log(a)
    return a
  }

  const setSavedTime = async (time) => {
    // send a request to /setTime with the time as the body
    const response = await fetch(`/setTime?time=${time}&key=${hexKey}` + time, {
      method: 'POST',
    
     
    });
  }
  getSavedTime().then((time) => {
    alarmTime = time;
  });
  

  const testBeep = () => {
    //send a request to /beep
    fetch(`/beep?key=${hexKey}`);

  };
  const getLockedState = () => {
    // if the time of day is between 9am and 6pm, return false
    const currentHour = new Date().getHours();
    return currentHour < 9 || currentHour > 18;
  }

  const dampen = () => {
    //send a request to /snooze
    fetch(`/dampen?key=${hexKey}`);
  };

  // Update current time every second
  setInterval(() => {
    currentTime = new Date().toLocaleTimeString();
    locked = getLockedState()
    if (locked) {
      // get minutes till 6pm or 9am, whichever is closest
      const currentHour = new Date().getHours();
      const minutesTill6pm = (6 - currentHour) * 60;
      const minutesTill9am = (9 - currentHour) * 60;
      lockedIn = Math.min(minutesTill6pm, minutesTill9am).toString();
      //convert lockedIn to hours
      lockedIn = (lockedIn / 60).toString();
      
    }
  }, 1000);
</script>

<style>
  
  .alarm-clock-container {  
    display: flex;
    flex-direction: column;
    justify-content: center;
    align-items: center;
    height: 100%;
    width: 100%;
    background: linear-gradient(135deg, #89f7fe 0%, #66a6ff 100%);
  }

  .alarm-clock {
    background: #ffffff;
    padding: 2rem;
    border-radius: 15px;
    box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
    display: flex;
    flex-direction: column;
    align-items: center;
  }

  input, button {
    margin: 0.5rem 0;
    padding: 0.5rem;
    border: 1px solid #ccc;
    border-radius: 5px;
    min-width: 6rem;
  }

  .buttons-container {
    display: flex;
    flex-direction: column;
  }

  button {
    cursor: pointer;
    background-color: #646cff;
    color: white;
    border: none;
  }

  button:hover {
    background-color: #535bf2;
  }
</style>
<div class="alarm-clock-container">
  <div class="alarm-clock">
 
    <h1>DAVID-ALARM 3000</h1>
    <p>Current Time: {currentTime}</p>
    <input type="time" bind:value={alarmTime} on:change={() => setSavedTime(alarmTime)} />
    <button on:click={testBeep}>Test Beep</button>
    <button on:click={dampen}>Dampen</button>
    {#if !locked}
    <div id="lockables">
      <!-- a numerical field for how long the alarm should beep, how many times it'll beep, the delay between each beep -->
      <!-- <input type="number" id="beep-length" placeholder="Beep Length (ms)" bind:value={beepLength} on:change={() => updateLocks()}>
      <input type="number" id="beep-count" placeholder="Beep Count" bind:value={beepCount} on:change={() => updateLocks()}>
      <input type="number" id="beep-delay" placeholder="Beep Delay (ms)" bind:value={beepDelay} on:change={() => updateLocks()}> -->
    </div>
    {:else}
      <p>Alarm will be unlocked in {lockedIn} hours</p>
    {/if}
  </div>
</div>
