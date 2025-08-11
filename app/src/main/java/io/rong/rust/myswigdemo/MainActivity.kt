package io.rong.rust.myswigdemo

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.tooling.preview.Preview
import io.rong.imlib.IMClient
import io.rong.imlib.base.callback.IData1Callback
import io.rong.imlib.base.enums.EngineError
import io.rong.imlib.connect.enums.ConnectionStatus
import io.rong.imlib.connect.listener.ConnectionStatusListener
import io.rong.imlib.conversation.enums.ConversationType
import io.rong.imlib.message.Message
import io.rong.imlib.message.callback.ISendMessageCallback
import io.rong.imlib.message.listener.MessageReceivedListener
import io.rong.imlib.message.model.ReceivedInfo
import io.rong.rust.myswigdemo.ui.theme.MySwigDemoTheme

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        init()
        connect()
        enableEdgeToEdge()
        setContent {
            MySwigDemoTheme {
                Scaffold(modifier = Modifier.fillMaxSize()) { innerPadding ->
                    Greeting(
                        name = "Android",
                        modifier = Modifier.padding(innerPadding)
                    )
                }
            }
        }
    }

    fun init() {
        IMClient.getInstance().init(baseContext, "n19jmcy59f1q9")
        setListener()
    }

    fun setListener() {
        IMClient.getInstance().setMessageReceivedListener(object : MessageReceivedListener {
            override fun onReceived(msg: Message?, info: ReceivedInfo?) {
                println("qxb setMessageReceivedListener onReceived ")
            }
        });
    }

    fun connect() {
        IMClient.getInstance().setConnectionStatusListener(object :
            ConnectionStatusListener {
            override fun onConnectionStatusChanged(status: ConnectionStatus?) {
                println("qxb onConnectionStatusChanged " + status)
            }
        })

        val token =
            "rdIKubNd6vTWuKUdlUU6eOF+lwb3rejulGH0HEWstV0=@h4mx.cn.rongnav.com;h4mx.cn.rongcfg.com"
        IMClient.getInstance().connect(token, 5, object : IData1Callback<String> {
            override fun onSuccess(value: String) {
                println("qxb connect onSuccess " + value)
                sendMessage()
            }

            override fun onError(error: EngineError) {
                println("qxb connect onError " + error)
            }
        })

    }

    fun sendMessage() {
        var msg = Message()
        msg.conversationType = ConversationType.PRIVATE
        msg.targetId = "test_target_id"
        msg.channelId = "test_channel_id"
        msg.objectName = "RC:TxtMsg"
        msg.contentJson = "{\"content\" : \"测试\"}"
        IMClient.getInstance().sendMessage(msg, object : ISendMessageCallback<Message> {
            override fun onError(error: EngineError?, data: Message?) {
//                TODO("Not yet implemented")
            }

            override fun onAttached(message: Message?) {
//                TODO("Not yet implemented")
            }

            override fun onSuccess(message: Message?) {
//                TODO("Not yet implemented")
            }

        })
    }

}

@Composable
fun Greeting(name: String, modifier: Modifier = Modifier) {
    Text(
        text = "Hello $name!",
        modifier = modifier
    )
}

@Preview(showBackground = true)
@Composable
fun GreetingPreview() {
    MySwigDemoTheme {
        Greeting("Android")
    }
}