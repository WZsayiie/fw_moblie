using UnityEngine;

public class GameRoot : MonoBehaviour
{
    void Start()
    {
        DontDestroyOnLoad(this);
    }
}
