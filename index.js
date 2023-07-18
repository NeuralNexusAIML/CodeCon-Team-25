

const express=require("express")
const mongoose=require("mongoose")
const app=express()
mongoose.set('strictQuery', true);

mongoose.connect('mongodb+srv://vaheedpasha1729:j91beRGsCjzSF1V8@iotcluster.xthzsjj.mongodb.net/test', {useNewUrlParser: true, useUnifiedTopology: true}, (err)=>{
    if(err)
        console.log('Error connecting to database', err);
    else
        console.log('Connected to database');
});

const cpsSchema = new mongoose.Schema({
    slot_id:{
        type:String,
        required:true
    },
    status:{
        type:String,
        required:true
    }

});

const CPS = mongoose.model('cps', cpsSchema);


const abc=new CPS({
    slot_id:'A1',
    status:'0'
})
//abc.save();

app.get('/',(req,res)=>{
    res.send("API CarPS running")
})


const request = require('request');

app.use((req, res, next) => {
  res.header('Access-Control-Allow-Origin', '*');
  next();
});

app.get('/abcd', (req, res) => {
  request(
    { url: 'https://carpsapi-production.up.railway.app/showdb' },
    (error, response, body) => {
      if (error || response.statusCode !== 200) {
        return res.status(500).json({ type: 'error', message: err.message });
      }

      res.json(JSON.parse(body));
    }
  )
});
app.get('/showdb',(req,res)=>{
    CPS.find({},(err,suc)=>{
        res.send(suc)
    })
})
app.post('/updatedb/:id/:status',(req,res)=>{
    const updateddb={
        slot_id:req.params.id,
        status:req.params.status
    }
    CPS.findOneAndUpdate({slot_id:req.params.id},updateddb,(err,suc)=>{
        if(err){
            console.log("unable to upadte db")
        }
        else{
            console.log("updated the db")
        }
    })
    res.send("success")
})

app.get('/newdb/:id/:status',(req,res)=>{
    const newdb={
        slot_id:req.params.id,
        status:req.params.status
    }
    CPS.insertMany(newdb);
})

app.get('/updatedb/:id/:status',(req,res)=>{
    console.log(req.params.id)
    const updateddb={
        slot_id:req.params.id,
        status:req.params.status
    }
    CPS.findOneAndUpdate({slot_id:req.params.id},updateddb,(err,suc)=>{
        if(err){
            console.log("unable to upadte db")
        }
        else{
            console.log("updated the db")
        }
    })
    res.send("success")
})



app.listen(process.env.PORT || 8000,()=>{
    console.log("server up and running")
})

