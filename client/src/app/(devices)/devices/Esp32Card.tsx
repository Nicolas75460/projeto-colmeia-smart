export default function Esp32Card() {
  return (
    <div className="device-card">
      <h2>ESP32</h2>
      <p>Status: <span className="offline">Offline</span></p>
      <p>Última atualização: há 10 min</p>
      <button className="device-btn">Ver detalhes</button>
    </div>
  );
}
