module.exports = [
  {
    "type": "heading",
    "defaultValue": "Configuration"
  },
     {
        "type": "text",
        "defaultValue": "<h6>A white toggle = OFF, an orange toggle = ON</h6>",
      },
	{
    "type": "section",
    "items": [
 	{
        "type": "heading",
        "defaultValue": "<h5>Display</h5>"
	},
     {
        "type": "color",
        "messageKey": "hrcol",
        "defaultValue": "0xFF0000",
        "label": "Hour Hand Color"
      },
      {
        "type": "color",
        "messageKey": "mincol",
        "defaultValue": "0x0000FF",
        "label": "Minute Hand Colour"
      },
      {
        "type": "color",
        "messageKey": "seccol",
        "defaultValue": "0xFF0000",
        "label": "Second Hand Colour"
	  },
	  {
        "type": "toggle",
        "messageKey": "secs",
        "label": "Show second hand",
        "defaultValue": true
      },
			{
  "type": "select",
  "messageKey": "background",
  "defaultValue": 0,
  "label": "Watch style",
  "options": [

    { 
      "label": "All Hieroglyphs",
      "value": 0
    },
    { 
      "label": "with main numbers",
      "value": 1
    },
    { 
      "label": "Star signs",
      "value": 2
    }

  ]
  
}
		
	]		
},

  {
        "type": "text",
        "defaultValue": "<h6>If you find this watchface useful, please consider making a <a href='https://www.paypal.me/markchopsreed'>small donation here</a>. Thankyou.</h6>",
  },
  {
    "type": "submit",
    "defaultValue": "Save Settings"
  }
];