window.onload = function() {
    const hello = document.getElementById('hello');
    let colors = ['#1e90ff', '#ff69b4', '#32cd32', '#ffa500', '#8a2be2'];
    let i = 0;
    setInterval(() => {
        hello.style.color = colors[i % colors.length];
        i++;
    }, 700);
};
