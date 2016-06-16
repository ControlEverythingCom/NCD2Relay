module['exports'] = function echoHttp (hook) {
  post('https://api.particle.io/v1/devices/#ENTER_DEVICE_ID_HERE#controlRelay',{arg : '1flicker', access_token : 'ENTER_YOUR_PARTICLE_ACCOUNT_ACCESS_TOKEN'})
};

function post(path, params, method) {
  	var querystring = require('querystring');
  	var https = require('https');
  
  	var post_data = querystring.stringify(params);
  	var post_options = {
      host: 'api.particle.io',
      port: '443',
      path: '/v1/devices/#ENTER_DEVICE_ID_HERE#/controlRelay',
      method: 'POST',
      headers: {
          'Content-Type': 'application/x-www-form-urlencoded'
      }
  };

  // Set up the request
  var post_req = https.request(post_options, function(res) {
      res.setEncoding('utf8');
      res.on('data', function (chunk) {
          //console.log('Response: ' + chunk);
      });
  });

  // post the data
  
  post_req.write(post_data);
  post_req.end();
}