$('.error-page').hide(0);

$('.try-again').click(function(){
  $('.error-page').hide(0);
  $('.login').slideDown(1000);
});