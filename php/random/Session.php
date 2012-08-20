<?php


class Session
{
	public $hash = null;
	public $id = null;
	public $information = null;

	private $database = null;

	public function __construct($database)
	{
		$this->database = $database;
		$this->hash = ((isset($_COOKIE['hash'])) ? $_COOKIE['hash'] : null);
		$this->information = array();

		if ($this->hash)
		{ $this->get(); }

		if (!$this->id)
		{ $this->set(); }
	}

	public function __get($name)
	{ return ((isset($this->information[$name])) ? $this->information[$name] : null); }

	public function __set($name, $value)
	{
		$this->information[$name] = $value;

		$query = 'update session set information = ? where sessionId = ?';

		if ($statement = $this->database->prepare($query))
		{
			$statement->bind_param('si', json_encode($this->information), $this->id);
			$statement->execute();
			$statement->close();
		}

		return;
	}

	public function delete(&$sessionProduct)
	{
		$query = 'delete from session where sessionId = ?';

		if ($statement = $this->database->prepare($query))
		{
			$statement->bind_param('i', $this->id);
			$statement->execute();
			$orderId = $statement->insert_id;
			$statement->close();
		}

		$sessionProduct->delete($this->id);

		$this->hash = null;
		$this->id = null;
		$this->information = array();

		return;
	}

	private function get()
	{
		$query = 'select sessionId, information from session where hash = ?';

		if ($statement = $this->database->prepare($query))
		{
			$statement->bind_param('s', $this->hash);
			$statement->execute();
			$statement->store_result();

			if ($statement->num_rows)
			{
				$statement->bind_result($sessionId, $information);
				$statement->fetch();

				$this->id = $sessionId;
				if ($information)
				{ $this->information = json_decode($information, 1); }

				$statement->free_result();
				$statement->close();
			}
			else
			{
				$statement->free_result();
				$statement->close();
				$this->set();
			}
		}

		return;
	}

	private function set()
	{
		$name = 'hash';
		$value = hash('md5', microtime(1), false);
		$expire = (time() + 2592000);
		$path = '/';
		$domain = '.' . DOMAIN;
		$secure = false;
		$httponly = true;

		setcookie($name, $value, $expire, $path, $domain, $secure, $httponly);

		$query = 'insert into session values (null, ?, null)';

		if ($statement = $this->database->prepare($query))
		{
			$statement->bind_param('s', $value);
			$statement->execute();
			$this->id = $statement->insert_id;
			$statement->close();
		}

		return;
	}
}

?>