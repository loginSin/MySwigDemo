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
import io.rong.rust.myswigdemo.ui.theme.MySwigDemoTheme

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        test()
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

    fun test() {
        IMClient.getInstance().init(baseContext, "n19jmcy59f1q9")

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
            }

            override fun onError(error: EngineError) {
                println("qxb connect onError " + error)
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