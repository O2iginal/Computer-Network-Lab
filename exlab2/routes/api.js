var express = require('express');
var router = express.Router();

/* GET api listing. */
router.get('/version', function(req, res, next) {
  res.send({ 'version': '1.0.0' });
});

router.get('/echo', function(req, res, next) {
  res.send({ 'name': req.query.name });
});

router.get('/sum', function(req, res, next) {
  var a = parseFloat(req.query.a);
  var b = parseFloat(req.query.b);
  res.send({ 'sum': a + b });
  // http://localhost:3000/api/sum?a=1.23&b=2
});

module.exports = router;
